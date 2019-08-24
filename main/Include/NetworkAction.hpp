//==============================================================================
// Copyright © 2019 Big Ass Fans
//==============================================================================
#ifndef NETWORK_ACTION_HEADER_INCLUDED
#define NETWORK_ACTION_HEADER_INCLUDED

#include <cstdint>
#include <memory>

#include "Action.hpp"
#include "ActionType.hpp"
//==============================================================================
// Public defines and constants
//==============================================================================

//==============================================================================
// Public typedefs
//==============================================================================
enum class NetworkActionType
{
    None,
    WifiConnected,
    WifiDisconnected,
    Server,
    Data,
    Provision,
    ClientConnect,
    ClientDisconnect,
    AcessPointList,
    CloudRegistration,
    CloudConnect,
    RegistrationResult,
    CloudDisconnected,
    GotIP
};
//==============================================================================
// Public variables
//==============================================================================

//==============================================================================
// Public function prototypes
//==============================================================================
class NetworkAction : public Action, public ActionType<NetworkActionType>
{
public:
    typedef struct
    {
        bool isServer;
    } ServerType;

    typedef struct
    {
        uint32_t                Port;
        ServerType              Type;
        uint32_t                Handle;
        bool                    IsSecured;

    } ServerInfo_t;
    typedef struct
    {
        uint32_t   handle;
        size_t     DataLength;
        ServerType Type;
        uint8_t *  Data;
    } Data_t;


    NetworkAction(NetworkActionType action) : mActionType(action){};
    //void     SetAccessPointList(ApList *list);
    void     SetServerInfo(ServerInfo_t &);
    void     SetSsid(uint8_t *, size_t size);
    void     SetPassphrase(uint8_t *, size_t size);
    void     SetIsAp(bool isAp);
    void     SetCloudConnectInfo(char *url, char *deviceId, char *cloudId);
    void     SetCloudRegistrationInfo(char *url, char *deviceId);
    void     SetCloudId(char *);
    void     SetData(uint8_t *, uint32_t size, ServerType, uint32_t handle);
    char *   GetDeviceId();
    char *   GetCloudId();
    char *   GetUrl();
    void     SetIpAddress(uint32_t);
    uint32_t GetIpAddress();
    void                Accept(Store &) override;
    const ServerInfo_t &GetServerInfo() const;
    const uint8_t *     GetSsid() const;
    size_t              GetSsidLength() const;
    const uint8_t *     GetPassphrase() const;
    size_t              GetPassphraseLength() const;
    const Data_t &      GetData() const;
    bool                GetIsAp() const;
    //ApList &            GetAccessPointList();

private:
    NetworkActionType mActionType{};
    ServerInfo_t      mServerInfo{};
    Data_t            mData{};
    size_t            mSsidLength{};
    size_t            mPassphraseLength{};
    bool              mIsAp{};
    uint8_t *         mSsid{nullptr};
    uint8_t *         mPassphrase{nullptr};
    //ApList *          mAccessPointList{nullptr};
    char *            mUrl{nullptr};
    char *            mDeviceId{nullptr};
    char *            mCloudId{nullptr};
    uint32_t          mIpAddress{0};
};
#endif    // NETWORK_ACTION_HEADER_INCLUDED
