--Lists the names of songs that are by Post Malone:
SELECT s.name FROM songs AS s
JOIN artists AS a ON a.id = s.artist_id
WHERE a.name = "Post Malone";

--Alternative:

--SELECT s.name FROM songs AS s, artists AS a
--WHERE s.artist_id = a.id
--AND a.name = "Post Malone";