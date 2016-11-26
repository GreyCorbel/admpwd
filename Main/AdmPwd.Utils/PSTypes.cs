using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.DirectoryServices.Protocols;
using System.Security.Principal;
using System.Xml.Linq;
using System.Linq;

namespace AdmPwd.PSTypes
{
    #region Enums

    public enum SchemaObjectType
    {
        Attribute = 0,
        Class
    }

    public enum PasswordResetState
    {
        PasswordReset = 0
    }

    public enum PermissionDelegationState
    {
        Unknown = 0,
        Delegated
    }

    public enum DirectoryOperationType
    {
        AddSchemaAttribute = 0,
        ModifySchemaClass
    }

    #endregion

    #region Output data
    public class ExtendedRightsInfo
    {
        public string ObjectDN { get { return DN; } }
        public ReadOnlyCollection<string> ExtendedRightHolders
        {
            get
            {
                return new ReadOnlyCollection<string>(erh);
            }
        }
        public ExtendedRightsInfo(string dn,List<string> holders)
        {
            DN = dn;
            erh = holders;
        }
        string DN;
        List<string> erh;
    }

    public class PasswordInfo
    {
        private string _computerName;
        private string _distinguishedName;
        private string _password;
        private DateTime _expirationTimestamp;

        public string ComputerName
        {
            get { return _computerName; }
            set { _computerName = value; }
        }

        public string DistinguishedName
        {
            get { return _distinguishedName; }
            set { _distinguishedName = value; }
        }

        public string Password
        {
            get { return _password; }
            set { _password = value; }
        }

        public DateTime ExpirationTimestamp
        {
            get { return _expirationTimestamp; }
            set { _expirationTimestamp = value; }
        }
        public PasswordInfo(string DistinguishedName)
        {
            this._distinguishedName = DistinguishedName;
        }
    }

    public class ObjectInfo
    {
        public string Name { get { return N; } }
        public string DistinguishedName { get { return DN; } }
        public PermissionDelegationState Status { get { return PDS; } }

        public ObjectInfo(string name, string dn, PermissionDelegationState status)
        {
            N = name;
            DN = dn;
            PDS = status;
        }

        string N;
        string DN;
        PermissionDelegationState PDS;
    }

    public class ForestInfo
    {
        private DomainInfo _rootDomain;
        private string _configurationNamingContext;
        private string _schemaNamingContext;

        public DomainInfo RootDomain
        {
            get { return _rootDomain; }
            set { _rootDomain = value; }
        }

        public string ConfigurationNamingContext
        {
            get { return _configurationNamingContext; }
            set { _configurationNamingContext = value; }
        }

        public string SchemaNamingContext
        {
            get { return _schemaNamingContext; }
            set { _schemaNamingContext = value; }
        }
        public ForestInfo()
        {
            _rootDomain = new DomainInfo();
        }
    }

    public class DomainInfo
    {
        private string _dnsName;
        private string _connectedHost;
        private SecurityIdentifier _sid;
        private string _dn;

        public string DnsName
        {
            get { return _dnsName; }
            set { _dnsName = value; }
        }

        public string Dn
        {
            get { return _dn; }
            set { _dn = value; }
        }

        public string ConnectedHost
        {
            get { return _connectedHost; }
            set { _connectedHost = value; }
        }

        public SecurityIdentifier Sid
        {
            get { return _sid; }
            set { _sid = value; }
        }
    }

    public class PasswordResetStatus
    {
        public string DistinguishedName { get { return DN; } }
        public PasswordResetState Status { get { return PRS; } }

        public PasswordResetStatus(string dn, PasswordResetState state)
        {
            DN = dn;
            PRS = state;
        }

        string DN;
        PasswordResetState PRS;
    }

    public class DirectoryOperationStatus
    {
        public DirectoryOperationType Operation
        {
            get
            {
                return DOT;
            }
        }
        public string DistinguishedName
        {
            get
            {
                return DN;
            }
        }
        public ResultCode Status
        {
            get
            {
                return RC;
            }
        }

        public DirectoryOperationStatus(DirectoryOperationType op, string distinguishedName, ResultCode state)
        {
            DOT = op;
            DN = distinguishedName;
            RC = state;
        }

        DirectoryOperationType DOT;
        string DN;
        ResultCode RC;
    }
    #endregion

    #region Exceptions
    [Serializable]
    public class AmbiguousResultException : Exception
    {
        public AmbiguousResultException(string message)
            : base(message)
        {
            
        }
    }

    [Serializable]
    public class NotFoundException : Exception
    {
        public NotFoundException(string message)
            : base(message)
        {

        }
    }
    #endregion
}