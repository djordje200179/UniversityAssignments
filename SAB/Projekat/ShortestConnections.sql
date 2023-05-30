CREATE VIEW ShortestConnections AS
WITH AllConnections(startCity, currentCity, path, distance) AS (
	SELECT idCity, idCity, CAST(idCity as varchar(1000)), 0
	FROM City
						
	UNION ALL
						
	SELECT
		startCity, idCity2,
		CAST(path + '|' + CAST(idCity2 as varchar(1000)) AS varchar(1000)),
		distance + Road.length
	FROM Road JOIN AllConnections ON currentCity = Road.idCity1
	WHERE path NOT LIKE '%' + CAST(idCity2 AS varchar(1000)) + '%'
)
SELECT A1.startCity, A1.currentCity as endCity, A1.distance, A1.path
FROM AllConnections A1
WHERE A1.distance = (
	SELECT MIN(A2.distance)
	FROM AllConnections A2
	WHERE A2.startCity = A1.startCity AND A2.currentCity = A1.currentCity
)