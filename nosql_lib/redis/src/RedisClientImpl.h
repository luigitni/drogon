/**
 *
 *  @file RedisClientImpl.h
 *  @author An Tao
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  https://github.com/an-tao/drogon
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Drogon
 *
 */
#pragma once

#include "RedisConnection.h"
#include <drogon/nosql/RedisClient.h>
#include <trantor/utils/NonCopyable.h>
#include <trantor/net/EventLoopThreadPool.h>
#include <vector>
#include <unordered_set>

namespace drogon
{
namespace nosql
{
class RedisClientImpl : public RedisClient,
                        public trantor::NonCopyable,
                        public std::enable_shared_from_this<RedisClientImpl>
{
  public:
    RedisClientImpl(const trantor::InetAddress &serverAddress,
                    const size_t numberOfConnections,
                    const std::string &password = "");
    virtual void execCommandAsync(RedisResultCallback &&resultCallback,
                                  RedisExceptionCallback &&exceptionCallback,
                                  string_view command,
                                  ...) noexcept override;
    virtual ~RedisClientImpl();

  private:
    trantor::EventLoopThreadPool loops_;
    std::mutex connectionsMutex_;
    std::unordered_set<RedisConnectionPtr> connections_;
    std::vector<RedisConnectionPtr> readyConnections_;
    size_t connectionPos_{0};
    RedisConnectionPtr newConnection(trantor::EventLoop *loop);
    const trantor::InetAddress serverAddr_;
    const std::string password_;
    const size_t numberOfConnections_;
};
}  // namespace nosql
}  // namespace drogon