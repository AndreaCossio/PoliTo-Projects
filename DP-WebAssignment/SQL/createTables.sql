DROP TABLE IF EXISTS `seatmap`;
CREATE TABLE `seatmap` (
    `seatId` varchar(5) NOT NULL,
    `userId` int(11) NOT NULL,
    `status` varchar(16) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
    `userId` int(11) NOT NULL,
    `email` varchar(256) NOT NULL,
    `password` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `seatmap`
    ADD PRIMARY KEY (`seatId`);

ALTER TABLE `users`
    ADD PRIMARY KEY (`userId`),
    ADD UNIQUE KEY `email` (`email`);

ALTER TABLE `users`
    MODIFY `userId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;