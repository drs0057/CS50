SELECT DISTINCT (name)
FROM movies
JOIN people
ON stars.person_id = people.id
JOIN stars
ON movies.id = stars.movie_id
WHERE year = 2004
ORDER BY birth ASC;