/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2014 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#ifndef MAPNIK_JSON_FEATURE_GENERATOR_GRAMMAR_HPP
#define MAPNIK_JSON_FEATURE_GENERATOR_GRAMMAR_HPP

// mapnik
#include <mapnik/feature.hpp>
#include <mapnik/geometry_container.hpp>
#include <mapnik/json/geometry_generator_grammar.hpp>
#include <mapnik/json/properties_generator_grammar.hpp>

// boost
#include <boost/spirit/include/karma.hpp>

namespace boost { namespace spirit { namespace traits {

template <>
struct is_container<mapnik::feature_impl const> : mpl::false_ {} ;

template <>
struct container_iterator<mapnik::feature_impl const>
{
    using type = mapnik::feature_kv_iterator2;
};

template <>
struct begin_container<mapnik::feature_impl const>
{
    static mapnik::feature_kv_iterator2
    call (mapnik::feature_impl const& f)
    {
        return mapnik::feature_kv_iterator2(mapnik::value_not_null(),f.begin(),f.end());
    }
};

template <>
struct end_container<mapnik::feature_impl const>
{
    static mapnik::feature_kv_iterator2
    call (mapnik::feature_impl const& f)
    {
        return mapnik::feature_kv_iterator2(mapnik::value_not_null(),f.end(),f.end());
    }
};

template <>
struct transform_attribute<const boost::fusion::cons<mapnik::feature_impl const&, boost::fusion::nil>,
                           mapnik::geometry_container const& ,karma::domain>
{
    using type = mapnik::geometry_container const&;
    static type pre(const boost::fusion::cons<mapnik::feature_impl const&, boost::fusion::nil>& f)
    {
        return boost::fusion::at<mpl::int_<0> >(f).paths();
    }
};

}}}

namespace mapnik { namespace json {

namespace karma = boost::spirit::karma;

struct get_id
{
    template <typename T>
    struct result { using type =  int; };

    int operator() (mapnik::feature_impl const& f) const
    {
        return f.id();
    }
};

template <typename OutputIterator>
struct feature_generator_grammar :
        karma::grammar<OutputIterator, mapnik::feature_impl const&()>
{
    feature_generator_grammar();
    karma::rule<OutputIterator, mapnik::feature_impl const&()> feature;
    multi_geometry_generator_grammar<OutputIterator, mapnik::geometry_container> geometry;
    properties_generator_grammar<OutputIterator, mapnik::feature_impl> properties;
    boost::phoenix::function<get_id> id_;
};

}}

#endif // MAPNIK_JSON_FEATURE_GENERATOR_GRAMMAR_HPP
