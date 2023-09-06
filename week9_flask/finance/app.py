import datetime
import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Prepare to calculate all values needed for table

    # Who is the current user
    user_dict = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])
    user = user_dict[0]["username"]

    # First, total cash the user has
    funds_dict = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    funds = usd(funds_dict[0]["cash"])

    # Gather a list of dictionaries; each will have unique tickers with shares owned
    tickers = db.execute("SELECT ticker, SUM(shares_change) as total_shares "
                         "FROM transactions WHERE user = ? GROUP BY ticker HAVING total_shares > 0", user)

    # Add to each dictionary the name, the current price of that ticker, individual/total valuation
    valuation = 0
    for dict in tickers:
        lookup_dict = lookup(dict["ticker"])
        valuation += lookup_dict["price"] * dict["total_shares"]
        dict["valuation"] = usd(lookup_dict["price"] * dict["total_shares"])
        dict["current_price"] = lookup_dict["price"]
        dict["name"] = lookup_dict["name"]

    total_account = usd(funds_dict[0]["cash"] + valuation)

    return render_template("portfolio.html", cash=funds, tickers=tickers, total_account=total_account)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Gather user input
        symbol = request.form.get("symbol").strip()
        shares = request.form.get("shares").strip()

        # Check if ticker input is blank or nonexistent
        if not symbol:
            return apology("Must enter a ticker", 400)
        elif not lookup(symbol):
            return apology("Must enter a valid ticker", 400)

        # Check if shares is a whole, numerical input greater than 0
        if not shares:
            return apology("Must input a number of shares", 400)

        elif not is_valid_number(shares):
            return apology("Must enter a positive, whole number of shares", 400)

        elif int(shares) <= 0:
            return apology("Must enter a positive, whole number of shares", 400)

        # Input is valid; see if purchase is possible

        # Gather total cost of purchase
        shares = int(shares)
        symbol_dict = lookup(symbol)
        price_per_share = symbol_dict["price"]
        cost = price_per_share * shares

        # Check if user has enough funds
        funds = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        funds = funds[0]["cash"]
        if funds < cost:
            return apology("You do not have enough funds", 400)

        # Purchase is possible; now gather all information needed to store in table "transactions":

        # What is the time
        time = datetime.datetime.now()
        time = time.strftime("%D:%H:%M:%S")

        # Who is the current user
        user_dict = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])
        user = user_dict[0]["username"]

        # Capitalize the ticker
        symbol_capitalize = symbol.upper()

        # Now input information into the table
        type = 'Purchase'
        db.execute("INSERT INTO transactions"
                   "(user, ticker, price_per_share, shares_change, time, cash_change, type)"
                   "VALUES (?, ?, ?, ?, ?, ?, ?)", user, symbol_capitalize, price_per_share, shares, time, -cost, type)

        # Update the user's cash
        update_cash = funds - cost
        db.execute("UPDATE users SET cash = ? WHERE id = ?", update_cash, session["user_id"])

        return redirect("/")

    # Method was GET
    else:
        return render_template("buy.html")


@app.route("/history")  # Method will be GET
@login_required
def history():
    """Show history of transactions"""

    # Who is the current user
    user_dict = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])
    user = user_dict[0]["username"]

    # Create a list dictionaries containing relevant information
    transactions_dict = db.execute("SELECT ticker, shares_change, price_per_share, type, time "
                                   "FROM transactions WHERE user = ? ORDER BY time DESC", user)

    return render_template("history.html", transactions=transactions_dict)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        # Use lookup to gather dict info from ticker
        ticker = request.form.get("symbol")

        # Check if ticker is valid
        ticker_dict = lookup(ticker)
        if not ticker_dict:
            return apology("Invalid ticker", 400)

        # Parse info into variables
        name = ticker_dict["name"]
        price = usd(ticker_dict["price"])
        symbol = ticker_dict["symbol"]

        # Render quoted page
        return render_template("quoted.html", name=name, price=price, symbol=symbol)

    # Method was GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Gather all input fields
        username = request.form.get("username")
        password = request.form.get("password")
        confirm = request.form.get("confirmation")

        # Check for null or already existing username
        usernames = [row['username'] for row in db.execute("SELECT username FROM users")]
        if not username:
            return apology("Username field cannot be blank.", 400)
        elif username in usernames:
            return apology("Username already taken.", 400)

        # Check for null or not matching passwords
        elif not password or not confirm:
            return apology("Please fill out both password fields.", 400)
        elif password != confirm:
            return apology("Passwords must match.", 400)

        # Registry is good; hash password and insert information into db
        hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)

        # User is registered, now they must login
        return render_template("login.html")

    # Route accessed via GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Who is the current user
    user_dict = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])
    user = user_dict[0]["username"]

    # Gather a list of dictionaries; each will have unique tickers with shares owned
    tickers = db.execute("SELECT ticker, SUM(shares_change) as total_shares "
                         "FROM transactions WHERE user = ? GROUP BY ticker HAVING total_shares > 0", user)

    # Gather dict of all stocks owned with respective shares
    stocks_owned = {}
    for dict in tickers:
        stocks_owned[dict["ticker"]] = dict["total_shares"]

    if request.method == "POST":

        # Gather user inputs
        symbol = request.form.get("symbol")
        sell_shares = request.form.get("shares").strip()

        # Check if an owned stock was selected
        if not symbol:
            return apology("Must enter a ticker", 400)
        symbol.strip()
        symbol = symbol.upper()
        if symbol not in stocks_owned:
            return apology("You do not own this stock", 400)

        # Check if shares input is greater than 0
        if not sell_shares:
            return apology("Must input a number of shares", 400)
        sell_shares = int(float(sell_shares))
        if sell_shares <= 0:
            return apology("Must enter a positive, whole number of shares", 400)

        # Check if user has enough shares to sell
        shares_owned = stocks_owned[symbol]
        if sell_shares > shares_owned:
            return apology("You do not have enough shares", 400)

        # Sale is valid, prepare to log appropriate values

        # Calculate profit made
        lookup_dict = lookup(symbol)
        price_per_share = lookup_dict["price"]
        profit = sell_shares * price_per_share

        # Update the user's cash
        db.execute("UPDATE users SET cash = cash + ? WHERE username = ?", profit, user)

        # Add the sale to transactions

        # What is the time
        time = datetime.datetime.now()
        time = time.strftime("%D:%H:%M:%S")

        # Now input information into the table
        type = 'Sale'
        db.execute("INSERT INTO transactions"
                   "(user, ticker, price_per_share, shares_change, time, cash_change, type)"
                   "VALUES (?, ?, ?, ?, ?, ?, ?)", user, symbol, price_per_share, -sell_shares, time, profit, type)

        # Redirect back to home page
        return redirect("/")

    # Method was GET
    else:
        return render_template("sell.html", tickers=tickers)


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Allows user to deposit cash to their account"""

    # Who is the current user
    user_dict = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])
    user = user_dict[0]["username"]

    if request.method == "POST":

        # Gather user input
        add_cash = request.form.get("deposit")

        # Check if input is blank or nonexistent
        if not add_cash:
            return apology("Must enter an amount", 400)

        # Check if input is greater than 0
        add_cash = float(add_cash)
        if add_cash <= 0:
            return apology("Must enter a positive amount", 400)

        # Deposit is valid, now add to user's cash
        db.execute("UPDATE users SET cash = cash + ? WHERE username = ?", add_cash, user)

        return redirect("/")

    # Method was GET
    else:
        return render_template("deposit.html")


def is_valid_number(value):
    try:
        int(value)
        return True
    except ValueError:
        return False