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

#ifndef INCLUDED_GNURADIO_TOP_BLOCK_HPP
#define INCLUDED_GNURADIO_TOP_BLOCK_HPP

#include <gnuradio/hier_block.hpp>

namespace gnuradio
{

struct GRAS_API TopBlock : HierBlock
{
    TopBlock(void);

    TopBlock(const std::string &name);

    /*!
     * Commit changes to the overall flow graph.
     * Call this after modifying connections.
     * Update is called automatically by start/stop/run.
     */
    void update(void);

    /*!
     * Set the buffer allocation hint.
     * This affects the size of buffers.
     */
    void set_buffer_hint(const size_t hint);

    //! Combined hint + start
    void start(const size_t hint)
    {
        this->set_buffer_hint(hint);
        this->start();
    }

    //! Combined hint + run
    void run(const size_t hint)
    {
        this->set_buffer_hint(hint);
        this->run();
    }

    /*!
     * Run is for finite flow graph executions.
     * Mostly for testing purposes only.
     */
    void run(void);

    //! Start a flow graph execution (does not block)
    void start(void);

    //! Stop a flow graph execution (does not block)
    void stop(void);

    /*!
     * Wait for threads to exit after stop() or run().
     * This is a blocking call and will not return until
     * all blocks in the topology have been marked done.
     */
    virtual void wait(void);

    /*!
     * Wait for threads to exit after stop() or run().
     * This is call will block until timeout or done.
     *
     * \param timeout the timeout in seconds
     * \return true of execution completed
     */
    virtual bool wait(const double timeout);

};

} //namespace gnuradio

#endif /*INCLUDED_GNURADIO_TOP_BLOCK_HPP*/
