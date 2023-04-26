#include <gtest/gtest.h>
#include "User.h"
#include "UserFactory.h"
#include "UserFactoryManager.h"
#include "Database.h"

// Tests for class Admin
TEST(AdminTest, ActionTest)
{
    Admin admin;
    EXPECT_EQ(admin.action(), "Admin action");
}

// Tests for class Mod
TEST(ModTest, ActionTest)
{
    Mod mod;
    EXPECT_EQ(mod.action(), "Mod action");
}

// Tests for class NormalUser
TEST(NormalUserTest, ActionTest)
{
    NormalUser user;
    EXPECT_EQ(user.action(), "Normal user action");
}

// Tests for class AdminFactory
TEST(AdminFactoryTest, CreateUserTest)
{
    AdminFactory factory;
    User* user = factory.createUser();
    EXPECT_EQ(user->action(), "Admin action");
    delete user;
}

// Tests for class ModFactory
TEST(ModFactoryTest, CreateUserTest)
{
    ModFactory factory;
    User* user = factory.createUser();
    EXPECT_EQ(user->action(), "Mod action");
    delete user;
}

// Tests for class NormalUserFactory
TEST(NormalUserFactoryTest, CreateUserTest)
{
    NormalUserFactory factory;
    User* user = factory.createUser();
    EXPECT_EQ(user->action(), "Normal user action");
    delete user;
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}