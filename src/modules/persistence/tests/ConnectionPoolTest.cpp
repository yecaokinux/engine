/**
 * @file
 */

#include "core/tests/AbstractTest.h"
#include "persistence/ConnectionPool.h"

namespace persistence {

class ConnectionPoolTest : public core::AbstractTest {
};

TEST_F(ConnectionPoolTest, testConnectionPoolSize) {
	ConnectionPool& pool = ConnectionPool::get();
	ASSERT_EQ(2, pool.init());
	pool.shutdown();
}

TEST_F(ConnectionPoolTest, testConnectionPoolGetConnection) {
	ConnectionPool& pool = ConnectionPool::get();
	ASSERT_EQ(2, pool.init());
	Connection* c = pool.connection();
	ASSERT_NE(nullptr, c);
	pool.shutdown();
}

TEST_F(ConnectionPoolTest, testConnectionPoolInvalidData) {
	ConnectionPool& pool = ConnectionPool::get();
	ASSERT_EQ(2, pool.init("invalid", "invalid", "invalid", "invalid"));
	ASSERT_EQ("invalid", core::Var::get(cfg::DatabaseName)->strVal());
	Connection* c = pool.connection();
	ASSERT_EQ(nullptr, c);
	pool.shutdown();
}

}