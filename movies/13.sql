--List the names of all people who starred in a movie in which Kevin Bacon also starred:
--* The query outputs a table with a single column for the name of each person.
--* We make sure to only select the Kevin Bacon born in 1958 (There may be multiple people named Kevin Bacon in the database)
--* We use a nNested query inside the WHERE statement: https://learnsql.com/blog/sql-nested-select/
--* Finaly, we remove Kevin Bacon himself from the resulting list.

SELECT p.name FROM people AS p
JOIN stars AS s ON s.person_id = p.id
WHERE s.movie_id IN (
        SELECT movie_id FROM stars AS s
        JOIN people AS p ON p.id = s.person_id
        WHERE p.name = "Kevin Bacon" AND p.birth = 1958
    )
    AND NOT (p.name = "Kevin Bacon" AND p.birth = 1958);