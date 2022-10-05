--List all movies released in 2010 and their ratings, in descending order by rating. For movies with the same rating, order them alphabetically by title:
--* The query outputs a table with two columns, one for the title of each movie and one for the rating of each movie.
--* Movies that do not have ratings are not included in the result.

SELECT title, rating FROM movies AS m
JOIN ratings AS r ON r.movie_id = m.id
WHERE year = 2010
AND rating NOT NULL
ORDER BY rating DESC, title;