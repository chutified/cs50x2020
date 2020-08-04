SELECT movies.title
FROM movies
JOIN ratings ON ratings.movie_id = movies.id
WHERE movies.id IN
(SELECT stars.movie_id
FROM stars
JOIN people ON people.id = stars.person_id
WHERE people.name = 'Chadwick Boseman')
ORDER BY ratings.rating DESC
LIMIT 5;