--List the names of the top 5 longest songs (descending order of length):
SELECT name FROM songs
ORDER BY duration_ms DESC
LIMIT 5;