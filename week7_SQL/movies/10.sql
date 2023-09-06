SELECT DISTINCT(name)
FROM people
JOIN ratings
ON movies.id = ratings.movie_id
JOIN movies
ON directors.movie_id = movies.id
JOIN directors
ON people.id = directors.person_id
WHERE rating >= 9.0;
