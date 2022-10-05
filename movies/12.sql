--List the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred:
--* The query outputs a table with a single column for the title of each movie.
--* We may assume that there is only one person in the database with the name Johnny Depp. Same for Helena Bonham Carter.
--* If we get a duplicated title in the results (`HAVING COUNT(m.title) > 1`), it means that both actors have played in this movie. Ref: https://learnsql.com/blog/how-to-find-duplicate-values-in-sql/

SELECT DISTINCT m.title FROM movies AS m
JOIN stars AS s ON s.movie_id = m.id
JOIN people AS p ON p.id = s.person_id
WHERE p.name IN ('Johnny Depp', 'Helena Bonham Carter')
GROUP BY m.title
HAVING COUNT(m.title) > 1;