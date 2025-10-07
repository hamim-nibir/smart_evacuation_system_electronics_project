-- create DB (if not exists)
CREATE DATABASE IF NOT EXISTS corridor_db CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE corridor_db;

-- sensor table (store readings)
CREATE TABLE IF NOT EXISTS sensor_data (
  id INT AUTO_INCREMENT PRIMARY KEY,
  corridor_id TINYINT NOT NULL,
  flame INT DEFAULT NULL,
  gas INT DEFAULT NULL,
  temperature FLOAT DEFAULT NULL,
  humidity FLOAT DEFAULT NULL,
  recorded_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  INDEX (corridor_id),
  INDEX (recorded_at)
);
