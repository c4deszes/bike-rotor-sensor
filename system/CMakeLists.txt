cmake_minimum_required(VERSION 3.20)
project(System VERSION 1.0.0 LANGUAGES NONE)

find_package(Python COMPONENTS Interpreter Development)

set(DOCS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/docs)
add_custom_target(
	docs ALL
	COMMAND sphinx-build -E ${DOCS_DIR} ${DOCS_DIR}/_build
)
