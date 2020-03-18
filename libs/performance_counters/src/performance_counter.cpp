//  Copyright (c) 2007-2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/config.hpp>
#include <hpx/apply.hpp>
#include <hpx/assertion.hpp>
#include <hpx/async.hpp>
#include <hpx/errors.hpp>
#include <hpx/functional/bind.hpp>

#include <hpx/performance_counters/counters.hpp>
#include <hpx/performance_counters/performance_counter.hpp>
#include <hpx/runtime/actions/continuation.hpp>

#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
namespace hpx { namespace performance_counters {

    ///////////////////////////////////////////////////////////////////////////
    performance_counter::performance_counter(std::string const& name)
      : base_type(performance_counters::get_counter_async(name))
    {
    }

    performance_counter::performance_counter(
        std::string const& name, hpx::id_type const& locality)
    {
        HPX_ASSERT(naming::is_locality(locality));

        counter_path_elements p;
        get_counter_path_elements(name, p);

        std::string full_name;
        p.parentinstanceindex_ = naming::get_locality_id_from_id(locality);
        get_counter_name(p, full_name);

        this->base_type::reset(
            performance_counters::get_counter_async(full_name));
    }

    ///////////////////////////////////////////////////////////////////////////
    future<counter_info> performance_counter::get_info() const
    {
        using action_type =
            server::base_performance_counter::get_counter_info_action;
        return hpx::async<action_type>(get_id());
    }
    counter_info performance_counter::get_info(
        launch::sync_policy, error_code& ec) const
    {
        return get_info().get(ec);
    }

    future<counter_value> performance_counter::get_counter_value(bool reset)
    {
        using action_type =
            server::base_performance_counter::get_counter_value_action;
        return hpx::async<action_type>(get_id(), reset);
    }
    counter_value performance_counter::get_counter_value(
        launch::sync_policy, bool reset, error_code& ec)
    {
        return get_counter_value(reset).get(ec);
    }

    future<counter_value> performance_counter::get_counter_value() const
    {
        using action_type =
            server::base_performance_counter::get_counter_value_action;
        return hpx::async<action_type>(get_id(), false);
    }
    counter_value performance_counter::get_counter_value(
        launch::sync_policy, error_code& ec) const
    {
        return get_counter_value().get(ec);
    }

    future<counter_values_array> performance_counter::get_counter_values_array(
        bool reset)
    {
        using action_type =
            server::base_performance_counter::get_counter_values_array_action;
        return hpx::async<action_type>(get_id(), reset);
    }
    counter_values_array performance_counter::get_counter_values_array(
        launch::sync_policy, bool reset, error_code& ec)
    {
        return get_counter_values_array(reset).get(ec);
    }

    future<counter_values_array> performance_counter::get_counter_values_array()
        const
    {
        using action_type =
            server::base_performance_counter::get_counter_values_array_action;
        return hpx::async<action_type>(get_id(), false);
    }
    counter_values_array performance_counter::get_counter_values_array(
        launch::sync_policy, error_code& ec) const
    {
        return get_counter_values_array().get(ec);
    }

    ///////////////////////////////////////////////////////////////////////////
    future<bool> performance_counter::start()
    {
        using action_type = server::base_performance_counter::start_action;
        return hpx::async<action_type>(get_id());
    }
    bool performance_counter::start(launch::sync_policy, error_code& ec)
    {
        return start().get(ec);
    }

    future<bool> performance_counter::stop()
    {
        using action_type = server::base_performance_counter::stop_action;
        return hpx::async<action_type>(get_id());
    }
    bool performance_counter::stop(launch::sync_policy, error_code& ec)
    {
        return stop().get(ec);
    }

    future<void> performance_counter::reset()
    {
        using action_type =
            server::base_performance_counter::reset_counter_value_action;
        return hpx::async<action_type>(get_id());
    }
    void performance_counter::reset(launch::sync_policy, error_code& ec)
    {
        reset().get(ec);
    }

    future<void> performance_counter::reinit(bool reset)
    {
        using action_type = server::base_performance_counter::reinit_action;
        return hpx::async<action_type>(get_id(), reset);
    }
    void performance_counter::reinit(
        launch::sync_policy, bool reset, error_code& ec)
    {
        reinit(reset).get(ec);
    }

    ///
    future<std::string> performance_counter::get_name() const
    {
        return lcos::make_future<std::string>(get_info(),
            [](counter_info&& info) -> std::string { return info.fullname_; });
    }

    std::string performance_counter::get_name(
        launch::sync_policy, error_code& ec) const
    {
        return get_name().get(ec);
    }

    ///////////////////////////////////////////////////////////////////////////
    /// Return all counters matching the given name (with optional wild-cards).
    std::vector<performance_counter> discover_counters(
        std::string const& name, error_code& ec)
    {
        std::vector<performance_counter> counters;

        std::vector<counter_info> infos;
        counter_status status =
            discover_counter_type(name, infos, discover_counters_full, ec);
        if (!status_is_valid(status) || ec)
            return counters;

        try
        {
            counters.reserve(infos.size());
            for (counter_info const& info : infos)
            {
                performance_counter counter(info.fullname_);
                counters.push_back(counter);
            }
        }
        catch (hpx::exception const& e)
        {
            HPX_RETHROWS_IF(ec, e, "discover_counters");
        }

        return counters;
    }
}}    // namespace hpx::performance_counters
