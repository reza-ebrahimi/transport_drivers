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

#include <gtest/gtest.h>

#include <rclcpp/rclcpp.hpp>

#include <chrono>
#include <string>
#include <vector>

#include "io_context/io_context.hpp"
#include "udp_driver/udp_driver.hpp"

using autoware::drivers::IoContext;
using autoware::drivers::UdpDriver;

const char ip[] = "127.0.0.1";
constexpr uint16_t port = 8000;

TEST(UdpDriverTest, NonBlockingSendReceiveTest)
{
  IoContext ctx;
  UdpDriver driver(ctx);

  driver.init_sender(ip, port);
  driver.init_receiver(ip, port);

  int32_t sum = 0;
  driver.receiver()->open();
  driver.receiver()->bind();
  driver.receiver()->asyncReceive(
    [&](const MutSocketBuffer & buffer) {
      sum += *reinterpret_cast<int32_t *>(buffer.data());
    });

  driver.sender()->open();
  EXPECT_EQ(driver.sender()->isOpen(), true);

  for (int val : {1, 2, 3, 4, 5}) {
    driver.sender()->asyncSend(MutSocketBuffer(&val, sizeof(val)));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  EXPECT_EQ(sum, 15);

  driver.sender()->close();
  EXPECT_EQ(driver.sender()->isOpen(), false);

  driver.receiver()->close();
  EXPECT_EQ(driver.receiver()->isOpen(), false);

  ctx.waitForExit();
}
