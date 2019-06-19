DROP TABLE IF EXISTS `s256681`.`seatmap`;

CREATE TABLE `s256681`.`seatmap` (
    `seatId` INT NOT NULL AUTO_INCREMENT,
    `row` INT NOT NULL,
    `col` CHAR NOT NULL,
    `userId` INT NULL,
    `status` VARCHAR(16) NOT NULL,
    PRIMARY KEY (`seatId`)
) ENGINE = InnoDB;

INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (1, 'A', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (1, 'B', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (1, 'C', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (1, 'D', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (1, 'E', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (1, 'F', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (2, 'A', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (2, 'B', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (2, 'C', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (2, 'D', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (2, 'E', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (2, 'F', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (3, 'A', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (3, 'B', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (3, 'C', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (3, 'D', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (3, 'E', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (3, 'F', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (4, 'A', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (4, 'B', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (4, 'C', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (4, 'D', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (4, 'E', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (4, 'F', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (5, 'A', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (5, 'B', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (5, 'C', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (5, 'D', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (5, 'E', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (5, 'F', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (6, 'A', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (6, 'B', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (6, 'C', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (6, 'D', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (6, 'E', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (6, 'F', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (7, 'A', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (7, 'B', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (7, 'C', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (7, 'D', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (7, 'E', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (7, 'F', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (8, 'A', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (8, 'B', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (8, 'C', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (8, 'D', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (8, 'E', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (8, 'F', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (9, 'A', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (9, 'B', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (9, 'C', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (9, 'D', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (9, 'E', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (9, 'F', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (10, 'A', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (10, 'B', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (10, 'C', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (10, 'D', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (10, 'E', NULL, 'free');
INSERT INTO `seatmap` (`row`, `col`, `userId`, `status`) VALUES (10, 'F', NULL, 'free');