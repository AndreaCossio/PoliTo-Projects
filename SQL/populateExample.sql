INSERT INTO `users` (`userId`, `email`, `password`) VALUES
(1, 'u1@p.it', '$2y$10$Qc9YwA49v78oNA20U38OBuTlKeTpovlvj8laHl9ENFhooAi7vLaRW'),
(2, 'u2@p.it', '$2y$10$I9PSV1BRzDnYJ6Tz/5hwyurUPMeXd/LNEWEVkZmk5UtKNLxWhUlCe');

INSERT INTO `seatmap` (`seatId`, `userId`, `status`) VALUES
('A4', 1, 'reserved'),
('B2', 2, 'purchased'),
('B3', 2, 'purchased'),
('B4', 2, 'purchased'),
('D4', 1, 'reserved'),
('F4', 2, 'reserved');