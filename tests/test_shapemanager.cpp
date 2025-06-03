#include <gtest/gtest.h>
#include <QVector>
#include "ShapeManager.h"

TEST(ShapeManager, DeltaSetters) {
    ShapeManager manager;
    TabData tab;
    manager.addTab(tab);
    EXPECT_EQ(manager.setDelataX(0, 5), 5);
    EXPECT_EQ(manager.getDelataX(0), 5);
    EXPECT_EQ(manager.setDelataY(0, 3), 3);
    EXPECT_EQ(manager.getDelataY(0), 3);
}

TEST(ShapeManager, AddShape) {
    ShapeManager manager;
    TabData tab;
    manager.addTab(tab);
    EXPECT_TRUE(manager.getShapes(0).empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
