/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include <ametsuchi/command_executor_stub.hpp>
#include <ametsuchi/ametsuchi_stub.hpp>

using iroha::ametsuchi::AmetsuchiStub;
using iroha::ametsuchi::CommandExecutorStub;
using namespace iroha::dao;

TEST(CommandExecutorTest, SampleTest) {
  AmetsuchiStub ametsuchi;
  CommandExecutorStub executor(ametsuchi);

  std::shared_ptr<Command> cmd = std::make_shared<AddPeer>();

  ASSERT_TRUE(executor.execute(*cmd));

  cmd = std::make_shared<Command>();

  ASSERT_FALSE(executor.execute(*cmd));
}