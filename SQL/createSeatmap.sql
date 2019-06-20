DROP TABLE IF EXISTS `s256681`.`seatmap`;

CREATE TABLE `s256681`.`seatmap` (
    `seatId` VARCHAR(5) NOT NULL,
    `userId` INT NOT NULL,
    `status` VARCHAR(16) NOT NULL,
    PRIMARY KEY (`seatId`)
) ENGINE = InnoDB;