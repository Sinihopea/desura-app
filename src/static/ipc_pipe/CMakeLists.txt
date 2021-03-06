include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/code
  ${SERVICECORE_INCLUDE_DIRS}
)

set(IPC_PIPE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/code PARENT_SCOPE)

file(GLOB Sources code/IPCClass.cpp
                  code/IPCManager.cpp
                  code/IPCParameter.cpp
				  code/IPCLockable.cpp
)

if(WIN32)
  file(GLOB PlattformSources code/IPCPipeBase.cpp
                             code/IPCPipeClient.cpp
                             code/IPCPipeServer.cpp
)
else()
  file(GLOB PlattformSources code/IPCPipeBase_Nix.cpp
                             code/IPCPipeClient_Nix.cpp
                             code/IPCPipeServer_Nix.cpp
)
endif()

add_library(ipc_pipe STATIC ${Sources} ${PlattformSources})

target_link_libraries(ipc_pipe
    threads
    util
    ${TINYXML_LIBRARIES}
)

add_dependencies(ipc_pipe tinyxml2)

if(WIN32)
  SetSharedRuntime(ipc_pipe)
endif()

SET_PROPERTY(TARGET ipc_pipe                PROPERTY FOLDER "Static")

if (WIN32)
	add_library(ipc_pipe_s STATIC ${Sources} ${PlattformSources})

	target_link_libraries(ipc_pipe_s
		threads_s
		util_s
		${TINYXML_LIBRARIES_S}
	)

	SET_PROPERTY(TARGET ipc_pipe_s                PROPERTY FOLDER "Static")
	add_dependencies(ipc_pipe_s tinyxml2_s)
	SetStaticRuntime(ipc_pipe_s)
endif()

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Weffc++")
endif()
