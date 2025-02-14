
/* Copyright (c) 2022 AntGroup. All Rights Reserved. */

#include "fma-common/configuration.h"
#include "lgraph/lgraph_rpc_client.h"
#include "monitor/prometheus_monitor.h"

int main(int argc, char** argv) {
    std::string server_host = "127.0.0.1:9091";
    std::string monitor_host = "127.0.0.1:9999";
    std::string user;
    std::string password;
    double sampling_interval_ms = 150;

    fma_common::Configuration config;
    config.ExitAfterHelp(true);
    config.Add(server_host, "server_host", true)
            .Comment("Host on which the tugraph rpc server runs.");
    config.Add(user, "u,user", false)
            .Comment("DB username");
    config.Add(password, "p,password", false)
            .Comment("DB password");
    config.Add(monitor_host, "monitor_host", true)
            .Comment("Host on which the monitor restful server runs.");
    config.Add(sampling_interval_ms, "sampling_interval_ms", true)
            .Comment("sampling interval in millisecond");
    config.ParseAndFinalize(argc, argv);

    lgraph::RpcClient client(server_host, user, password);
    lgraph::monitor::ResourceMonitor monitor(monitor_host);

    while (true) {
        usleep(sampling_interval_ms * 1000);
        std::string str;
        bool ret = client.CallCypher(str,
                                     "CALL db.monitor.tuGraphInfo()");
        if (ret) {
            monitor.report_tugraph_info(str);
        }
        ret = client.CallCypher(str, "CALL db.monitor.serverInfo()");
        if (ret) {
            monitor.report_server_info(str);
        }
    }
    return 0;
}
