--List the names of all people who starred in Toy Story:
--* The query outputs a table with a single column for the name of each person.
--* We may assume that there is only one movie in the database with the title Toy Story.

SELECT p.name FROM people AS p
JOIN stars AS s ON s.person_id = p.id
JOIN movies AS m ON m.id = s.movie_id
WHERE m.title = "Toy Story";