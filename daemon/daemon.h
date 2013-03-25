// Copyright (c) 2012, Cornell University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of HyperDex nor the names of its contributors may be
//       used to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef hyperdex_daemon_daemon_h_
#define hyperdex_daemon_daemon_h_

// STL
#include <tr1/memory>

// po6
#include <po6/net/hostname.h>
#include <po6/net/ipaddr.h>
#include <po6/net/location.h>
#include <po6/pathname.h>
#include <po6/threads/thread.h>

// Replicant
#include <replicant.h>

// HyperDex
#include "common/ids.h"
#include "daemon/communication.h"
#include "daemon/coordinator_link.h"
#include "daemon/datalayer.h"
#include "daemon/replication_manager.h"
#include "daemon/search_manager.h"
#include "daemon/state_transfer_manager.h"

#ifdef __APPLE__

#include <mach/mach_init.h>
#include <mach/x86_64/thread_act.h>
#include <sched.h>

#define cpu_set_t thread_affinity_policy_data_t

#define CPU_SET(cpu_id, new_mask) \
			*new_mask.affinity_tag = (cpu_id + 1)

#define CPU_ZERO(new_mask) \
			*new_mask.affinity_tag == THREAD_AFFINITY_TAG_NULL

#define SET_AFFINITY(pid, size, mask) \
  			thread_policy_set(mach_thread_self(), \
			THREAD_AFFINITY_POLICY, \
			mask, \
			THREAD_AFFINITY_POLICY_COUNT)

#define GET_AFFINITY(pid, size, mask) \
			thread_policy_get(mach_thread_self(), \
			THREAD_AFFINITY_POLICY, \
			mask, \
			THREAD_AFFINITY_POLICY_COUNT)

/*
#define pthread_setaffinity_np(pid, size, mask) \
			SET_AFFINITY(mach_thread_self(), size, mask)
*/
#define pthread_setaffinity_np(pid, size, mask) 0
#endif // __APPLE__


namespace hyperdex
{

class daemon
{
    public:
        daemon();
        ~daemon() throw ();

    public:
        int run(bool daemonize,
                po6::pathname data,
                bool set_bind_to,
                po6::net::location bind_to,
                bool set_coordinator,
                po6::net::hostname coordinator,
                unsigned threads);

    private:
        void loop(size_t thread);
        void process_req_get(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_req_atomic(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_req_search_start(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_req_search_next(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_req_search_stop(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_req_sorted_search(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_req_group_del(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_req_count(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_req_search_describe(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_chain_op(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_chain_subspace(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_chain_ack(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_chain_gc(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_xfer_op(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);
        void process_xfer_ack(server_id from, virtual_server_id vfrom, virtual_server_id vto, std::auto_ptr<e::buffer> msg, e::unpacker up);

    private:
        friend class communication;
        friend class coordinator_link;
        friend class datalayer;
        friend class replication_manager;
        friend class search_manager;
        friend class state_transfer_manager;

    private:
        server_id m_us;
        std::vector<std::tr1::shared_ptr<po6::threads::thread> > m_threads;
        coordinator_link m_coord;
        datalayer m_data;
        communication m_comm;
        replication_manager m_repl;
        state_transfer_manager m_stm;
        search_manager m_sm;
        configuration m_config;
};

} // namespace hyperdex

#endif // hyperdex_daemon_daemon_h_
