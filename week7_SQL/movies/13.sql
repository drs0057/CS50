SELECT name
FROM people
JOIN movies
ON stars.movie_id = movies.id
JOIN stars
ON people.id = stars.person_id
WHERE
title IN(
    SELECT title
    FROM movies
    JOIN people
    ON stars.person_id = people.id
    JOIN stars
    ON movies.id = stars.movie_id
    WHERE name = 'Kevin Bacon'
    AND
    birth = 1958
)
AND
NOT name = 'Kevin Bacon';
