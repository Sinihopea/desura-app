/*
Copyright (C) 2011 Mark Chandler (Desura Net Pty Ltd)
Copyright (C) 2014 Bad Juju Games, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.

Contact us at legal@badjuju.com.
*/

#ifndef DESURA_UPDATETHREAD_H
#define DESURA_UPDATETHREAD_H
#ifdef _WIN32
#pragma once
#endif

#include "util_thread/BaseThread.h"
#include "Event.h"
#include "mcfcore/ProgressInfo.h"

#include "util/gcTime.h"


namespace MCFCore
{
namespace Thread
{

//! This class handles the reporting of progress from multithreads of work.
//! It predicts the time left and also the rate of progress
//!
class UpdateProgThread : public ::Thread::BaseThread
{
public:
	UpdateProgThread(uint16 count, uint64 totSize);
	~UpdateProgThread();

	//! Reports completed progress from a worker thread
	//!
	//! @param id Worker id
	//! @param ammount Progress amount in bytes
	//!
	void reportProg(uint32 id, uint64 ammount);

	//! Reports that a thread has stopped and thus shouldnt be used in the predictions
	//!
	//! @param id Worker id
	//!
	void stopThread(uint32 id);

	//! Sets the total ammount of work to be done
	//!
	//! @param totSize Total size in bytes
	//!
	void setTotal(uint64 totSize);

	//! Sets the total done so far (i.e. resuming a download)
	//!
	//! @param doneSize Total done so far in bytes
	//!
	void setDone(uint64 doneSize);


	//! This is the event that gets triggered when it reports progress
	Event<MCFCore::Misc::ProgressInfo> onProgUpdateEvent;

protected:
	//! Main thread supper loop
	//!
	void run();

	//! Calculates the progress so far
	//!
	void calcResults();

	void onPause();
	void onStop();
	void onUnpause();

private:
	std::mutex m_pProgMutex;

	gcTime m_tLastUpdateTime;
	gcTime m_tStartTime;
	gcTime m_tPauseStartTime;
	gcDuration m_tTotPauseTime;

	uint16 m_pCount = 0;
	std::vector<uint64> m_vProgInfo;

	uint64 m_uiTotalSize = 0;
	uint64 m_uiDoneSize = 0;
};

}}

#endif //DESURA_UPDATETHREAD_H
