SELECT DISTINCT(people.name)
FROM people
JOIN directors ON directors.person_id = people.id
WHERE directors.movie_id IN
(SELECT movies.id
FROM movies
JOIN ratings ON ratings.movie_id = movies.id
WHERE ratings.rating >= 9.0);