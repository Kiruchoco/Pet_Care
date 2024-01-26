-- Adminer 4.8.1 MySQL 8.0.29-0ubuntu0.20.04.3 dump

SET NAMES utf8;
SET time_zone = '+00:00';
SET foreign_key_checks = 0;
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

CREATE TABLE `dev` (
  `seq` int NOT NULL AUTO_INCREMENT,
  `device` varchar(20) CHARACTER SET utf8mb3 COLLATE utf8_general_ci NOT NULL,
  `value` varchar(10) CHARACTER SET utf8mb3 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`seq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;


SET NAMES utf8mb4;

CREATE TABLE `log` (
  `seq` int NOT NULL,
  `txt` varchar(4096) COLLATE utf8mb4_general_ci NOT NULL,
  PRIMARY KEY (`seq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;


CREATE TABLE `timer` (
  `seq` int NOT NULL AUTO_INCREMENT COMMENT '일련번호',
  `title` varchar(100) NOT NULL COMMENT '알람 제목',
  `time` time NOT NULL COMMENT '시간',
  PRIMARY KEY (`seq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;


-- 2022-06-04 10:40:48
