SELECT DISTINCT(people.name)
FROM people
JOIN stars ON stars.person_id = people.id
WHERE stars.movie_id IN
(SELECT movies.id
FROM movies
JOIN stars ON stars.movie_id = movies.id
WHERE stars.person_id IS
(SELECT people.id
FROM people
WHERE people.name = "Kevin Bacon" AND people.birth = 1958
LIMIT 1))
AND people.name != "Kevin Bacon";