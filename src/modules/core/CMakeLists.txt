set(SRCS
	AABB.h
	command/CommandHandler.h command/CommandHandler.cpp
	command/Command.h command/Command.cpp
	trace/Remotery.c trace/Remotery.h
	trace/microprofile.h
	App.cpp App.h
	ByteStream.cpp ByteStream.h
	Color.cpp Color.h
	ConcurrentQueue.h
	Common.h
	Concurrency.h
	EventBus.cpp EventBus.h
	Frustum.cpp Frustum.h
	GameConfig.h
	GLM.cpp GLM.h
	IFactoryRegistry.h
	Input.cpp Input.h
	JSON.h json.hpp
	Log.cpp Log.h
	MD5.cpp MD5.h
	MemoryAllocator.h
	MemGuard.cpp MemGuard.h
	NonCopyable.h
	Plane.h Plane.cpp
	Process.cpp Process.h
	QuadTree.h
	Random.cpp Random.h
	ReadWriteLock.h
	Rect.h
	Set.h
	Singleton.h
	String.cpp String.h
	ThreadPool.cpp ThreadPool.h
	TimeProvider.h TimeProvider.cpp
	Tokenizer.h Tokenizer.cpp
	Trace.cpp Trace.h
	UTF8.cpp UTF8.h
	Var.cpp Var.h
	Vertex.h
	Zip.h
)
set(LIB core)
add_library(${LIB} ${SRCS})
target_link_libraries(${LIB} io zlib ${CMAKE_DL_LIBS})
set_target_properties(${LIB} PROPERTIES FOLDER ${LIB})

gtest_suite_files(tests
	tests/AbstractTest.cpp
	tests/CoreTest.cpp
	tests/ColorTest.cpp
	tests/RectTest.cpp
	tests/ByteStreamTest.cpp
	tests/ThreadPoolTest.cpp
	tests/EventBusTest.cpp
	tests/QuadTreeTest.cpp
	tests/VarTest.cpp
	tests/CommandTest.cpp
	tests/MD5Test.cpp
	tests/SetTest.cpp
	tests/TokenizerTest.cpp
	tests/FrustumTest.cpp
	tests/PlaneTest.cpp
)
gtest_suite_deps(tests ${LIB})

gtest_suite_begin(tests-core TEMPLATE ${ROOT_DIR}/src/modules/core/tests/main.cpp.in)
gtest_suite_files(tests-core
	tests/AbstractTest.cpp
	tests/CoreTest.cpp
	tests/RectTest.cpp
	tests/ByteStreamTest.cpp
	tests/ThreadPoolTest.cpp
	tests/EventBusTest.cpp
	tests/QuadTreeTest.cpp
	tests/VarTest.cpp
	tests/CommandTest.cpp
	tests/MD5Test.cpp
	tests/SetTest.cpp
	tests/TokenizerTest.cpp
	tests/FrustumTest.cpp
	tests/PlaneTest.cpp
)
gtest_suite_deps(tests-core ${LIB})
gtest_suite_end(tests-core)
