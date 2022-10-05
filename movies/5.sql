--List the titles and release years of all Harry Potter movies, in chronological order:
--* The query outputs a table with two columns, one for the title of each movie and one for the release year of each movie.
--* We may assume that the title of all Harry Potter movies will begin with the words “Harry Potter”.

SELECT title, year FROM movies
WHERE title LIKE "Harry Potter%"
ORDER BY year;