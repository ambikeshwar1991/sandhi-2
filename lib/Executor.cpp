//
// Copyright 2012 Josh Blum
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <Apology/Executor.hpp>
#include <ApologyVecUtils.hpp>

using namespace Apology;

Executor::Executor(Topology *topology)
{
    _topology = topology;
}

Executor::~Executor(void)
{
    _flat_flows.clear();
    _worker_set.clear();
}

void Executor::commit(void)
{
    std::vector<Flow> new_flat_flows;

    //step 1) resolve all flows in the topology
    {
        new_flat_flows = _topology->_resolve_flows();
    }

    //step 2) determine new flows
    const std::vector<Flow> flows_to_add = vector_subtract(new_flat_flows, _flat_flows);

    //step 3) add new inputs
    _worker_set.clear();
    for (size_t i = 0; i < flows_to_add.size(); i++)
    {
        const Flow &flow = flows_to_add[i];
        Worker *worker = reinterpret_cast<Worker *>(flow.dst.elem);
        vector_vector_add(worker->_inputs, flow.dst.index, flow.src);
        insert_unique(_worker_set, worker);
    }
    this->post_all(WorkerTopologyMessage());

    //step 4) add new outputs
    _worker_set.clear();
    for (size_t i = 0; i < flows_to_add.size(); i++)
    {
        const Flow &flow = flows_to_add[i];
        Worker *worker = reinterpret_cast<Worker *>(flow.src.elem);
        vector_vector_add(worker->_outputs, flow.src.index, flow.dst);
        insert_unique(_worker_set, worker);
    }
    this->post_all(WorkerTopologyMessage());

    //step 5) determine old flows
    const std::vector<Flow> flows_to_remove = vector_subtract(_flat_flows, new_flat_flows);

    //step 6) remove old outputs
    _worker_set.clear();
    for (size_t i = 0; i < flows_to_remove.size(); i++)
    {
        const Flow &flow = flows_to_remove[i];
        Worker *worker = reinterpret_cast<Worker *>(flow.src.elem);
        vector_vector_remove(worker->_outputs, flow.src.index, flow.dst);
        insert_unique(_worker_set, worker);
    }
    this->post_all(WorkerTopologyMessage());

    //step 7) remove old inputs
    _worker_set.clear();
    for (size_t i = 0; i < flows_to_remove.size(); i++)
    {
        const Flow &flow = flows_to_remove[i];
        Worker *worker = reinterpret_cast<Worker *>(flow.dst.elem);
        vector_vector_remove(worker->_inputs, flow.dst.index, flow.src);
        insert_unique(_worker_set, worker);
    }
    this->post_all(WorkerTopologyMessage());

    //step 8) update the flat flows
    _flat_flows = new_flat_flows;

    //step 9) get a unique list of blocks
    _worker_set.clear();
    _worker_set.reserve(_flat_flows.size());
    for (size_t i = 0; i < _flat_flows.size(); i++)
    {
        const Flow &flow = _flat_flows[i];
        insert_unique(_worker_set, reinterpret_cast<Worker *>(flow.src.elem));
        insert_unique(_worker_set, reinterpret_cast<Worker *>(flow.dst.elem));
    }
}
