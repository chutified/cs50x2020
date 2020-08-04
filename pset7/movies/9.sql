SELECT DISTINCT(people.name)
FROM people
JOIN stars ON stars.person_id = people.id
WHERE stars.movie_id IN
(SELECT movies.id
FROM movies
WHERE movies.year = 2004)
ORDER BY birth ASC;