DROP TABLE IF EXISTS `s256681`.`users`;

CREATE TABLE `s256681`.`users` (
    `userId` INT NOT NULL AUTO_INCREMENT,
    `email` VARCHAR(256) NOT NULL,
    `password` TEXT NOT NULL,
    PRIMARY KEY (`userId`),
    UNIQUE `email` (`email`)
) ENGINE = InnoDB;