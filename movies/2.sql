--Determine the birth year of Emma Stone:
--* The query outputs a table with a single column and a single row containing Emma Stone’s birth year.
--* We may assume that there is only one person in the database with the name Emma Stone.

SELECT birth FROM people
WHERE name == "Emma Stone";