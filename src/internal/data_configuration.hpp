//
//  data_configuration.hpp
//  blocksci_devel
//
//  Created by Harry Kalodner on 3/14/17.
//  Copyright © 2017 Harry Kalodner. All rights reserved.
//

#ifndef data_configuration_h
#define data_configuration_h

#include "chain_configuration.hpp"

#include <blocksci/core/typedefs.hpp>

#include <wjfilesystem/path.h>

#include <string>
#include <vector>

namespace blocksci {
    
    static constexpr int dataVersion = 5;
    
    nlohmann::json loadConfig(const std::string &configFilePath);
    void checkVersion(const nlohmann::json &jsonConf);

    /** Loads and holds blockchain configuration files, needed to load blockchains */
    struct DataConfiguration {
        DataConfiguration() {}

        DataConfiguration(const std::string &configPath, ChainConfiguration &config, bool errorOnReorg, BlockHeight blocksIgnored);
        DataConfiguration(const std::string &configPath, ChainConfiguration &config, bool errorOnReorg, BlockHeight blocksIgnored, std::shared_ptr<DataConfiguration> parentDataConfiguration);

        std::string configPath;
        bool errorOnReorg;

        /** Block height that BlockSci should load up to as a 1-indexed number
         * Eg. 10 loads blocks [0, 9], and -6 loads all but the last 6 blocks */
        BlockHeight blocksIgnored;

        /** Configuration of an individual chain, eg. coinName, dataDirectory, segwitActivationHeight etc. */
        ChainConfiguration chainConfig;

        std::shared_ptr<DataConfiguration> parentDataConfiguration;
        std::shared_ptr<DataConfiguration> childDataConfiguration;

        bool isNull() const {
            return chainConfig.dataDirectory.empty();
        }

        const DataConfiguration& rootDataConfiguration() const{
            const DataConfiguration* current = this;
            while (current->parentDataConfiguration != nullptr) {
                current = current->parentDataConfiguration.get();
            }
            return *current;
        }

        DataConfiguration& rootDataConfiguration() {
            DataConfiguration* current = this;
            while (current->parentDataConfiguration != nullptr) {
                current = current->parentDataConfiguration.get();
            }
            return *current;
        }
        
        filesystem::path scriptsDirectory() const {
            return this->rootDataConfiguration().chainConfig.dataDirectory/"scripts";
        }
        
        filesystem::path chainDirectory() const {
            return chainConfig.dataDirectory/"chain";
        }
        
        filesystem::path mempoolDirectory() const {
            return chainConfig.dataDirectory/"mempool";
        }
        
        filesystem::path addressDBFilePath() const {
            return this->rootDataConfiguration().chainConfig.dataDirectory/"addressesDb";
        }
        
        filesystem::path hashIndexFilePath() const {
            // return path of root chain, if available
            return this->rootDataConfiguration().chainConfig.dataDirectory/"hashIndex";
        }
        
        filesystem::path pidFilePath() const {
            return chainConfig.dataDirectory/"blocksci_parser.pid";
        }
        
        bool operator==(const DataConfiguration &other) const {
            return chainConfig.dataDirectory == other.chainConfig.dataDirectory;
        }
        
        bool operator!=(const DataConfiguration &other) const {
            return chainConfig.dataDirectory != other.chainConfig.dataDirectory;
        }
    };
    
    DataConfiguration loadBlockchainConfig(const std::string &configPath, bool errorOnReorg, BlockHeight blocksIgnored);
}

#endif /* data_configuration_h */
