#pragma once

namespace pt::post_processes
{
    struct clamp
    {
        template<typename Buffer_Type>
        [[nodiscard]] constexpr auto operator()(Buffer_Type&& buffer)
        {
            
        }
    };
}