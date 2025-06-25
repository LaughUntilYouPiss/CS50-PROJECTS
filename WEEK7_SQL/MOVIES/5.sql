SELECT title,
       year AS release_year
 FROM movies
WHERE title LIKE 'Harry Potter%'
ORDER BY year;
