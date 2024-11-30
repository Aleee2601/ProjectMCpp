-- --------------------------------------------------------
-- Host:                         localhost
-- Server version:               PostgreSQL 17.0 on x86_64-windows, compiled by msvc-19.41.34120, 64-bit
-- Server OS:                    
-- HeidiSQL Version:             12.8.0.6908
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES  */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Dumping structure for table public.Building
CREATE TABLE IF NOT EXISTS "Building" (
	"Building_id" INTEGER NOT NULL,
	"BuildingRegion_id" INTEGER NOT NULL,
	"Building_name" VARCHAR(50) NOT NULL
);

-- Dumping data for table public.Building: 8 rows
/*!40000 ALTER TABLE "Building" DISABLE KEYS */;
INSERT INTO "Building" ("Building_id", "BuildingRegion_id", "Building_name") VALUES
	(1, 1, 'Faleza '),
	(2, 1, 'Gradina Botanica'),
	(3, 1, 'Turnul TV'),
	(4, 1, 'Combinatul Siderurgic'),
	(5, 2, 'Faleza'),
	(6, 2, 'Teatrul Maria Filotti'),
	(7, 2, 'Port'),
	(8, 2, 'Parcul Monument');
/*!40000 ALTER TABLE "Building" ENABLE KEYS */;

-- Dumping structure for table public.Game
CREATE TABLE IF NOT EXISTS "Game" (
	"Game_id" INTEGER NOT NULL,
	"Game_start" TIMESTAMP NOT NULL,
	"Game_end" TIMESTAMP NULL DEFAULT NULL,
	"Game_region_win_id" INTEGER NULL DEFAULT NULL
);

-- Dumping data for table public.Game: 0 rows
/*!40000 ALTER TABLE "Game" DISABLE KEYS */;
/*!40000 ALTER TABLE "Game" ENABLE KEYS */;

-- Dumping structure for table public.Game_Building
CREATE TABLE IF NOT EXISTS "Game_Building" (
	"Game_Building_id" INTEGER NOT NULL,
	"Game_id" INTEGER NOT NULL,
	"Building_id" INTEGER NOT NULL,
	"Player_id" INTEGER NOT NULL
);

-- Dumping data for table public.Game_Building: 0 rows
/*!40000 ALTER TABLE "Game_Building" DISABLE KEYS */;
/*!40000 ALTER TABLE "Game_Building" ENABLE KEYS */;

-- Dumping structure for table public.Game_Player
CREATE TABLE IF NOT EXISTS "Game_Player" (
	"Game_Player_id" INTEGER NOT NULL,
	"Player_id" INTEGER NOT NULL,
	"Game_id" INTEGER NOT NULL,
	"Region_id" INTEGER NOT NULL,
	"Player_points" INTEGER NOT NULL
);

-- Dumping data for table public.Game_Player: 0 rows
/*!40000 ALTER TABLE "Game_Player" DISABLE KEYS */;
/*!40000 ALTER TABLE "Game_Player" ENABLE KEYS */;

-- Dumping structure for table public.Player
CREATE TABLE IF NOT EXISTS "Player" (
	"Player_id" INTEGER NOT NULL,
	"Nickname" VARCHAR(50) NOT NULL,
	"Password" VARCHAR(50) NOT NULL,
	"Points" INTEGER NOT NULL,
	"Score" INTEGER NOT NULL
);

-- Dumping data for table public.Player: 0 rows
/*!40000 ALTER TABLE "Player" DISABLE KEYS */;
/*!40000 ALTER TABLE "Player" ENABLE KEYS */;

-- Dumping structure for table public.Region
CREATE TABLE IF NOT EXISTS "Region" (
	"Region_id" INTEGER NOT NULL,
	"Region_name" VARCHAR(50) NOT NULL
);

-- Dumping data for table public.Region: 2 rows
/*!40000 ALTER TABLE "Region" DISABLE KEYS */;
INSERT INTO "Region" ("Region_id", "Region_name") VALUES
	(1, 'Galati'),
	(2, 'Braila');
/*!40000 ALTER TABLE "Region" ENABLE KEYS */;

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
