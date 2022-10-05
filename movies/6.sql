--Determine the average rating of all movies released in 2012:
--* The query outputs a table with a single column and a single row containing the average rating.

SELECT AVG(rating) FROM movies AS m
JOIN ratings AS r ON r.movie_id = m.id
WHERE m.year = 2012;