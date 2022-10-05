--List the titles of all movies with a release date on or after 2018, in alphabetical order:
--* The query outputs a table with a single column for the title of each movie.
--* Movies released in 2018 and later are included.

SELECT title FROM movies
WHERE year >= 2018
ORDER BY title;