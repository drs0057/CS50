-- Keep a log of any SQL queries you execute as you solve the mystery.


--First, lets read the description of the crime scene report from Jul 28, 2021 on Humphrey Street
SELECT description
FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28
AND street = 'Humphrey Street';
--NOTE: Theft at 10:15am at the Humphrey Street bakery. Three witnesses mentioning bakery, crime occured at bakery, littering ocurred at 16:36. Lets read interviews first.



                                        --Read interviews
SELECT transcript
FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28;
--Transcripts below:
    --1: Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
    --If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.

    --2:I don't know the thief's name, but it was someone I recognized.
    --Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

    --3:As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
    --In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
    --The thief then asked the person on the other end of the phone to purchase the flight ticket.




                    --Biggest lead: check earliest flight out of Fiftyville on July 29th, 2021. Keep this flight in mind, he could have flown on it.
SELECT *
FROM flights
WHERE year = 2021
AND month = 7
AND day = 29
ORDER BY hour ASC,
minute ASC LIMIT 1;

--Now gather origin airport and destination airport:
SELECT city
FROM airports
JOIN flights
ON airports.id = flights.origin_airport_id
WHERE year = 2021
AND month = 7
AND day = 29
ORDER BY hour ASC,
minute ASC LIMIT 1;
--Origin city: Fiftyville
SELECT city
FROM airports
JOIN flights
ON airports.id = flights.destination_airport_id
WHERE year = 2021
AND month = 7
AND day = 29
ORDER BY hour ASC,
minute ASC LIMIT 1;
--Destination city: New York City
--Now collect list of possible passport numbers and hold on to it:
SELECT passport_number
FROM passengers
JOIN flights
ON passengers.flight_id = flights.id
WHERE flight_id = (
    SELECT id
    FROM flights
    WHERE year = 2021
    AND month = 7
    AND day = 29
    ORDER BY hour ASC,
    minute ASC LIMIT 1
);



                            -- Next lead: security footage from bakery (within 10 minutes of 10:15am)


-- Gather all potential license plates. Hold on to them.
SELECT license_plate
FROM bakery_security_logs
WHERE year = 2021
AND month = 7
AND day = 28
AND hour = 10
AND minute >= 15
ORDER BY hour ASC,
minute ASC
LIMIT 8; --After 8 entries, time has passed 10:25am



                                --Next lead: Emma's bakery ATM withdrawl on Leggett Street, sometime in the morning of July 28th, 2021


--Gather all potential account numbers
SELECT account_number
FROM atm_transactions
WHERE year = 2021
AND month = 7
AND day = 28
AND transaction_type = 'withdraw'
AND atm_location = 'Leggett Street';
--We can use potential account numbers to gather list of potential people via bank_account:
SELECT name
FROM people
JOIN atm_transactions
ON bank_accounts.account_number = atm_transactions.account_number
JOIN bank_accounts
ON people.id = bank_accounts.person_id
WHERE atm_transactions.account_number
IN (
    SELECT account_number
    FROM atm_transactions
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND transaction_type = 'withdraw'
    AND atm_location = 'Leggett Street'
);
-- We now have potential lists of names, license plates, passport number. Lets explore phone calls.



                    --Next lead: phone call for less than one minute as theif was leaving the bakery. This occurred between 10:16am and 10:23am.



--Gather potential list of name from the callers.
SELECT name
FROM people
JOIN phone_calls
ON people.phone_number = phone_calls.caller
WHERE phone_number
IN (
    SELECT caller
    FROM phone_calls
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60
);



                    --Overlap this with all pools of data to find the thief. The receiver of that phone call will be the accomplice.


SELECT name
FROM people
WHERE name
IN (
    SELECT name
    FROM people
    JOIN atm_transactions
    ON bank_accounts.account_number = atm_transactions.account_number
    JOIN bank_accounts
    ON people.id = bank_accounts.person_id
    WHERE atm_transactions.account_number
    IN (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2021
        AND month = 7
        AND day = 28
        AND transaction_type = 'withdraw'
        AND atm_location = 'Leggett Street'
    )
)
AND passport_number
IN (
    SELECT passport_number
    FROM passengers
    JOIN flights
    ON passengers.flight_id = flights.id
    WHERE flight_id = (
        SELECT id
        FROM flights
        WHERE year = 2021
        AND month = 7
        AND day = 29
        ORDER BY hour ASC,
        minute ASC LIMIT 1
    )
)
AND license_plate
IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute >= 15
    ORDER BY hour ASC,
    minute ASC
    LIMIT 8
)
AND name
IN (
    SELECT *
    FROM people
    JOIN phone_calls
    ON people.phone_number = phone_calls.caller
    WHERE phone_number
    IN (
        SELECT caller
        FROM phone_calls
        WHERE year = 2021
        AND month = 7
        AND day = 28
        AND duration < 60
    )
);


                                --Bruce was the thief! Now lets use his phone number to find the accomplice.


SELECT name
FROM people
where phone_number = (
    SELECT receiver
    FROM phone_calls
    WHERE caller = (
        SELECT caller
        FROM phone_calls
        JOIN people
        ON phone_calls.caller = people.phone_number
        WHERE name = (
            SELECT name
            FROM people
            WHERE name
            IN (
                SELECT name
                FROM people
                JOIN atm_transactions
                ON bank_accounts.account_number = atm_transactions.account_number
                JOIN bank_accounts
                ON people.id = bank_accounts.person_id
                WHERE atm_transactions.account_number
                IN (
                    SELECT account_number
                    FROM atm_transactions
                    WHERE year = 2021
                    AND month = 7
                    AND day = 28
                    AND transaction_type = 'withdraw'
                    AND atm_location = 'Leggett Street'
                )
            )
            AND passport_number
            IN (
                SELECT passport_number
                FROM passengers
                JOIN flights
                ON passengers.flight_id = flights.id
                WHERE flight_id = (
                    SELECT id
                    FROM flights
                    WHERE year = 2021
                    AND month = 7
                    AND day = 29
                    ORDER BY hour ASC,
                    minute ASC LIMIT 1
                )
            )
            AND license_plate
            IN (
                SELECT license_plate
                FROM bakery_security_logs
                WHERE year = 2021
                AND month = 7
                AND day = 28
                AND hour = 10
                AND minute >= 15
                ORDER BY hour ASC,
                minute ASC
                LIMIT 8
            )
            AND name
            IN (
                SELECT name
                FROM people
                JOIN phone_calls
                ON people.phone_number = phone_calls.caller
                WHERE phone_number
                IN (
                    SELECT caller
                    FROM phone_calls
                    WHERE year = 2021
                    AND month = 7
                    AND day = 28
                    AND duration < 60
                )
            )
        )
    )
    AND year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60
);
-- The accomplice was Robin!!!

                                --All in all, Bruce was the thief, he flew to New York City, and Robin helped him.