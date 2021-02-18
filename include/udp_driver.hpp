// Copyright 2021 LeoDrive.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Developed by LeoDrive, 2021

#ifndef UDP_DRIVER_UDP_DRIVER_HPP
#define UDP_DRIVER_UDP_DRIVER_HPP

#include "io_context.hpp"
#include "udp_socket.hpp"

namespace autoware
{
namespace drivers
{

class UdpDriver {
public:
    UdpDriver(const IoContext &ctx);
    ~UdpDriver();

    void init_sender(const std::string &ip, uint16_t port);
    void init_receiver(const std::string &ip, uint16_t port);

    std::shared_ptr<UdpSocket> sender() const;
    std::shared_ptr<UdpSocket> receiver() const;

private:
    const IoContext &m_ctx;
    std::shared_ptr<UdpSocket> m_sender;
    std::shared_ptr<UdpSocket> m_receiver;
};

}  // namespace drivers
}  // namespace autoware

#endif //UDP_DRIVER_UDP_DRIVER_HPP
