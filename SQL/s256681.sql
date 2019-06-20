DROP TABLE IF EXISTS `seatmap`;
CREATE TABLE `seatmap` (
  `seatId` varchar(5) NOT NULL,
  `userId` int(11) NOT NULL,
  `status` varchar(16) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `seatmap` (`seatId`, `userId`, `status`) VALUES
('A4', 1, 'reserved'),
('B2', 2, 'purchased'),
('B3', 2, 'purchased'),
('B4', 2, 'purchased'),
('D4', 1, 'reserved'),
('F4', 2, 'reserved');

DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `userId` int(11) NOT NULL,
  `email` varchar(256) NOT NULL,
  `password` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `users` (`userId`, `email`, `password`) VALUES
(1, 'u1@p.it', '$2y$10$Qc9YwA49v78oNA20U38OBuTlKeTpovlvj8laHl9ENFhooAi7vLaRW'),
(2, 'u2@p.it', '$2y$10$I9PSV1BRzDnYJ6Tz/5hwyurUPMeXd/LNEWEVkZmk5UtKNLxWhUlCe');

ALTER TABLE `seatmap`
  ADD PRIMARY KEY (`seatId`);

ALTER TABLE `users`
  ADD PRIMARY KEY (`userId`),
  ADD UNIQUE KEY `email` (`email`);

ALTER TABLE `users`
  MODIFY `userId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;