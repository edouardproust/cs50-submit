--List the names of all people who have directed a movie that received a rating of at least 9.0:
--* The query outputs a table with a single column for the name of each person.
--* If a person directed more than one movie that received a rating of at least 9.0, they appear only once in the results.

SELECT p.name FROM people AS p
JOIN directors AS d ON d.person_id = p.id
JOIN ratings AS r ON r.movie_id = d.movie_id
WHERE r.rating >= 9;