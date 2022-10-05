--List the names of all people who starred in a movie released in 2004, ordered by birth year:
--* The query outputs a table with a single column for the name of each person.
--* People with the same birth year are listed in any order.
--* No worries about people who have no birth year listed, so long as those who do have a birth year are listed in order.
--* If a person appeared in more than one movie in 2004, they appear only once in the result.

SELECT DISTINCT p.name FROM people AS p
JOIN stars AS s ON s.person_id = p.id
JOIN movies AS m ON m.id = s.movie_id
WHERE m.year = 2004
ORDER BY p.birth;