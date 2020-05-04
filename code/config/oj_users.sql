-- ----------------------------
-- Table structure for oj_users
-- ----------------------------
DROP TABLE IF EXISTS `oj_users`;
CREATE TABLE `oj_users`  (
  `id` int(255) NOT NULL AUTO_INCREMENT,
  `email` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `password` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 13 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of oj_users
-- ----------------------------
INSERT INTO `oj_users` VALUES (0, '111@qq.com', 'm', '123');
INSERT INTO `oj_users` VALUES (12, '123@qq.com', '123', 'aaa');