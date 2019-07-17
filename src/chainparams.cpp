// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The Times 28/May/2013 Obama Plans 3 Nominations for Key Court";
    const CScript genesisOutputScript = CScript() << ParseHex("4104678afdb0fe5548261967f1a67130b7102cd6a828e03909a67962e0ea1f61deb649f6ca3f4cef38c4f35504e51ec102ee5c384df7ba0b8d578a4c702b6bf11d5fac") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.BIP16Height = 1 ; // 87afb798a3ad9378fcd56123c81fb31cfd9a8df4719b9774d71730c16315a092 - October 1, 2012
        consensus.BIP34Height = 370000;
        consensus.BIP34Hash = uint256S("30f12d7821d151a391ba29b1299f8b00c9b773158af4fe918cd36eb65dc4ce79"); // @todo americancoin
        consensus.BIP65Height = 371000; // bab3041e8977e0dc3eeff63fe707b92bde1dd449d8efafb248c27c8264cc311a @todo americancoin
        consensus.BIP66Height = 372000; // 7aceee012833fa8952f8835d8b1b3ae233cd6ab08fdb27a771d2bd7bdc491894 @todo americancoin
        consensus.Diff504ForkHeight = 14000;
        consensus.DiffDarkGravityWave3Height = 361800;
        consensus. DiffLWMAHeight = 382000;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = (7 * 24 * 60 * 60) / 8; // 21 hours
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 102 ; // > 50% of 200
        consensus.nMinerConfirmationWindow = 200; // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1893456000; // 2030
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1893456000 + (3600 * 1) ;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].forkHeight = 380000;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1893456000;// 2030
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1893456000 + (3600 * 2);
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].forkHeight = 381000;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000e54f57572ff67"); // update at block 370164

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xd376813ba680bc59f003abf60fcbd10ac4f7a36092786721502d8e9e051bfa76"); //1503191 // @todo americancoin

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x41; // 'A'
        pchMessageStart[1] = 0x4d; // 'M'
        pchMessageStart[2] = 0x43; // 'C'
        pchMessageStart[3] = 0x3a; // ':'
        nDefaultPort = 9056;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1369760071, 109478, 0x1e0ffff0, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x84409e2b69534476cfd1cecf030848abe98db872843498f179d755c627ce9a87"));
        assert(genesis.hashMerkleRoot == uint256S("0xe9a0047d082da09fda981cbbc7e32822b7ff8faa575564983c62d07059a9587f"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.


        vSeeds.emplace_back("americancoin.ignorelist.com");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,24); // A or B
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5); // 3
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,28); // C
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,152); // (24 + 128) : P
        base58Prefixes[EXT_PUBLIC_KEY] = {0x41, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x41, 0x88, 0xAD, 0xE4};

        bech32_hrp = "amc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                {  0, uint256S("0xe235425c8314df2f26b94d89fd5582edc74d80cf39d105107ea6cd12d8898491")},
                {  9000, uint256S("0x6aaddca66be07c4df9c5bb14ec5a46edfa13b0e208187b1275744e4354339a8b")},
                {  12150, uint256S("0x91f24da86e5aaf6cfcf146e62d6b6cdc63e1b1ce699f776399e311d707cc1aa6")},
                { 13550, uint256S("0x0c50d43e4982187e704c8c9d2b33d2617aed69ee471dffb744e58414c961cb11")},
                { 15121, uint256S("0x040e045718181b2467df5c2eb2cd3a95ee45240b54cbffc68ef00aa254a961a8")},
                { 50000, uint256S("0xd05ffe2d08469eaf8e84f50de97bda320ffa5467ce768e3b12e550dfc850c499")},
                { 100000, uint256S("0x804519b95b4f036a85e2f2a5c8bd53a04794a88df7e2df97e0359a71426bb075")},
                {150000, uint256S("0x7396ad19096bc56de1035d5bda81252af17a864a821643e8913eddf8c9ad9338")},
                {200000, uint256S("0x3d9ac91d5c4f753d7f70e7081fc857c0ddb0c510a0aef7d144ec63db3886a4bb")},
				{250000, uint256S("0xd8e87de02ec9e305511742f2aaf05e655b1d202966ba08175e20ff5be5af8132")},
				{300000, uint256S("0x69e652e43906a8f967f9aec1db38be3c8863a9323552ebba3f1f5c5fbb197e75")},
				{361800, uint256S("0xf9df1ed691866f77199800088d361cb2a6dd45069357a2a6547b62f1c9a32cdc")},
				{370000, uint256S("0x30f12d7821d151a391ba29b1299f8b00c9b773158af4fe918cd36eb65dc4ce79")},



            }
        };

        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 a601455787cb65ffc325dda4751a99cf01d1567799ec4b04f45bb05f9ef0cbde
            /* nTime    */ 1538637641,
            /* nTxCount */ 28086163,
            /* dTxRate  */ 0.334
        };

        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = false;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on testnet
        consensus.BIP34Height = 76;
        consensus.BIP34Hash = uint256S("8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573");
        consensus.BIP65Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.Diff504ForkHeight = 14000;
        consensus.DiffDarkGravityWave3Height = 361800;
        consensus. DiffLWMAHeight = 999999999; // Expected but not defined
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // 3.5 days
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1517356801; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000d59b4e7fb18a2f469f54a6d40f2195bebd626cd93e30b64710facc49036"); //787085

        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xd2;
        pchMessageStart[2] = 0xc8;
        pchMessageStart[3] = 0xf1;
        nDefaultPort = 19056;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1561292487, 2946844, 0x1e0ffff0, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000d59b4e7fb18a2f469f54a6d40f2195bebd626cd93e30b64710facc49036"));
        assert(genesis.hashMerkleRoot == uint256S("0xe9a0047d082da09fda981cbbc7e32822b7ff8faa575564983c62d07059a9587f"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top


        vSeeds.emplace_back("americancoin.ignorelist.com");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111); // m or n
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196); //
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,33); // 33
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x41, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x41, 0x35, 0x83, 0x94};

        bech32_hrp = "tamc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                //{2056, uint256S("17748a31ba97afdc9a4f86837a39d287e3e7c7290a08a1d816c5969c78a83289")},
            }
        };

        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 438b7e1c86f58b4e62e8cf2d0f9f256e3dddaebc5d1cc568e633a38e0db6c025
            /* nTime    */ 1538637952,
            /* nTxCount */ 1845705,
            /* dTxRate  */ 1.907
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.Diff504ForkHeight = 14000;
        consensus.DiffDarkGravityWave3Height = 361800;
        consensus. DiffLWMAHeight = 999999999; // Expected but not defined
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 0, 0x207fffff, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x530827f38f93b43ed12af0b3ad25a288dc02ed74d6d7857862df51fc56c416f9")); // @todo americancoin
        assert(genesis.hashMerkleRoot == uint256S("0xe9a0047d082da09fda981cbbc7e32822b7ff8faa575564983c62d07059a9587f")); // @todo americancoin

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = {
            {
                //{0, uint256S("530827f38f93b43ed12af0b3ad25a288dc02ed74d6d7857862df51fc56c416f9")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x41, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x41, 0x35, 0x83, 0x94};

        bech32_hrp = "ramc";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
