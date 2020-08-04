SELECT people.name
FROM people
JOIN stars ON stars.person_id = people.id
WHERE stars.movie_id IN
(SELECT movies.id
FROM movies
WHERE movies.title = "Toy Story");