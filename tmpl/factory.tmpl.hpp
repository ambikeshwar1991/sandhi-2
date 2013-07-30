// Copyright (C) by Josh Blum. See LICENSE.txt for licensing information.

#ifndef INCLUDED_GRAS_FACTORY_HPP
#define INCLUDED_GRAS_FACTORY_HPP

#include <gras/gras.hpp>
#include <gras/element.hpp>
#include <PMC/PMC.hpp>
#include <string>

namespace gras
{

/*!
 * Element factory:
 *  - Register factory functions into the global factory.
 *  - Call make() to create element from global factory.
 *
 * Example register a factory function:
 *  gras::Factory::register_make("/proj/my_block", &make_my_block);
 *
 * Example call into the factory:
 *  gras::Element *my_block = gras::Factory::make("/proj/my_block", arg0, arg1);
 */
struct GRAS_API Factory
{
    /*******************************************************************
     * Private registration hooks
     ******************************************************************/
    static void _register_make(const std::string &, void *);
    static Element *_handle_make(const std::string &, const PMCC &);
};

/***********************************************************************
 * Register API - don't look here, template magic, not helpful
 **********************************************************************/
#for $NARGS in range($MAX_ARGS)
template <typename ReturnType, $expand('typename A%d', $NARGS)>
static void register_make(const std::string &name, ReturnType(*fcn)($expand('const A%d &', $NARGS)));

#end for
/***********************************************************************
 * Make API - don't look here, template magic, not helpful
 **********************************************************************/
#for $NARGS in range($MAX_ARGS)
template <$expand('typename A%d', $NARGS)>
static Element *make(const std::string &name, $expand('const A%d &', $NARGS));

#end for
}

/*!
 * Register a block's factory function:
 * Declare this macro at the global scope in a cpp file.
 * The block will register at static initialization time.
 */
#define GRAS_REGISTER_FACTORY(name, fcn) \
    GRAS_STATIC_BLOCK(fcn) \
    {gras::register_make(name, &fcn);}

#for $NARGS in range($MAX_ARGS)
/*!
 * Register a block's constructor into the factory:
 * The arguments to this macro must be the types of each constructor argument.
 * Example: GRAS_REGISTER_FACTORY2("/proj/my_block", MyBlock, std::string, size_t)
 * Declare this macro at the global scope in a cpp file.
 * The block will register at static initialization time.
 */
#define GRAS_REGISTER_FACTORY$(NARGS)(name, type, $expand('A%d', $NARGS)) \
    static gras::Element *make_ $('##') type($expand('const A%d &a%d', $NARGS)) \
    { return new type($expand('a%d', $NARGS)); } \
    GRAS_REGISTER_FACTORY(name, make_$('##')type)

#end for
#include <gras/detail/factory.hpp>

#endif /*INCLUDED_GRAS_FACTORY_HPP*/
