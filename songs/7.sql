--Returns the average energy of songs that are by Drake:
SELECT AVG(energy) FROM songs AS s
JOIN artists AS a ON a.id = s.artist_id
WHERE a.name = "Drake";

--Alternative:

--SELECT AVG(energy) FROM songs AS s, artists AS a
--WHERE s.artist_id = a.id
--AND a.name = "Drake";