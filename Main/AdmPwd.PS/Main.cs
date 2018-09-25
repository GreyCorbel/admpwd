using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.DirectoryServices;
using System.DirectoryServices.Protocols;
using System.Management.Automation;
using  System.Runtime.InteropServices;
using AdmPwd.Utils;
using AdmPwd.PSTypes;

namespace AdmPwd.PS
{
    [Cmdlet("Update", "AdmPwdADSchema")]
    public class UpdateADSchema : Cmdlet
    {
        protected override void ProcessRecord()
        {
            ForestInfo fi = DirectoryUtils.GetForestRootDomain();
            using (LdapConnection conn = DirectoryUtils.GetLdapConnection(ConnectionType.Ldap))
            {
                AddRequest rqAdd;
                ModifyRequest rqMod;

                AddResponse rspAdd;
                ModifyResponse rspMod;

                //password timestamp
                rqAdd = new AddRequest();

                rqAdd.DistinguishedName = "cn=" + Constants.TimestampAttributeName + "," + fi.SchemaNamingContext;
                rqAdd.Attributes.Add(new DirectoryAttribute("ldapDisplayName", Constants.TimestampAttributeName));
                rqAdd.Attributes.Add(new DirectoryAttribute("adminDisplayName", Constants.TimestampAttributeName));

                rqAdd.Attributes.Add(new DirectoryAttribute("objectClass","attributeSchema"));
                rqAdd.Attributes.Add(new DirectoryAttribute("attributeId", "1.2.840.113556.1.8000.2554.50051.45980.28112.18903.35903.6685103.1224907.2.2"));
                rqAdd.Attributes.Add(new DirectoryAttribute("attributeSyntax", "2.5.5.16"));
                rqAdd.Attributes.Add(new DirectoryAttribute("omSyntax", "65"));
                rqAdd.Attributes.Add(new DirectoryAttribute("isSingleValued", "TRUE"));
                rqAdd.Attributes.Add(new DirectoryAttribute("systemOnly", "FALSE"));
                rqAdd.Attributes.Add(new DirectoryAttribute("isMemberOfPartialAttributeSet", "FALSE"));
                rqAdd.Attributes.Add(new DirectoryAttribute("searchFlags","0"));
                rqAdd.Attributes.Add(new DirectoryAttribute("showInAdvancedViewOnly", "FALSE"));

                try
                {
                    rspAdd = conn.SendRequest(rqAdd) as AddResponse;
                    WriteObject(new DirectoryOperationStatus(DirectoryOperationType.AddSchemaAttribute, rqAdd.DistinguishedName, rspAdd.ResultCode));
                }
                catch (DirectoryOperationException ex)
                {
                    if(ex.Response.ResultCode==ResultCode.EntryAlreadyExists)
                        WriteObject(new DirectoryOperationStatus(DirectoryOperationType.AddSchemaAttribute, rqAdd.DistinguishedName, ex.Response.ResultCode));
                    else
                        throw;
                }

                //password
                rqAdd = new AddRequest();
                rqAdd.DistinguishedName = "cn=" + Constants.PasswordAttributeName + "," + fi.SchemaNamingContext;
                rqAdd.Attributes.Add(new DirectoryAttribute("ldapDisplayName", Constants.PasswordAttributeName));
                rqAdd.Attributes.Add(new DirectoryAttribute("adminDisplayName", Constants.PasswordAttributeName));
                rqAdd.Attributes.Add(new DirectoryAttribute("objectClass", "attributeSchema"));
                rqAdd.Attributes.Add(new DirectoryAttribute("attributeId", "1.2.840.113556.1.8000.2554.50051.45980.28112.18903.35903.6685103.1224907.2.1"));
                rqAdd.Attributes.Add(new DirectoryAttribute("attributeSyntax", "2.5.5.5"));
                rqAdd.Attributes.Add(new DirectoryAttribute("omSyntax", "19"));
                rqAdd.Attributes.Add(new DirectoryAttribute("isSingleValued", "TRUE"));
                rqAdd.Attributes.Add(new DirectoryAttribute("systemOnly", "FALSE"));
                rqAdd.Attributes.Add(new DirectoryAttribute("isMemberOfPartialAttributeSet", "FALSE"));
                rqAdd.Attributes.Add(new DirectoryAttribute("searchFlags", "904"));
                rqAdd.Attributes.Add(new DirectoryAttribute("showInAdvancedViewOnly", "FALSE"));

                try
                {
                    rspAdd = conn.SendRequest(rqAdd) as AddResponse;
                    WriteObject(new DirectoryOperationStatus(DirectoryOperationType.AddSchemaAttribute, rqAdd.DistinguishedName, rspAdd.ResultCode));
                }
                catch (DirectoryOperationException ex)
                {
                    if (ex.Response.ResultCode == ResultCode.EntryAlreadyExists)
                        WriteObject(new DirectoryOperationStatus(DirectoryOperationType.AddSchemaAttribute, rqAdd.DistinguishedName, ex.Response.ResultCode));
                    else
                        throw;
                }
                
                //Reload schema
                rqMod = new ModifyRequest();
                DirectoryAttributeModification schemaReload=new DirectoryAttributeModification();
                schemaReload.Name="SchemaUpdateNow";
                schemaReload.Operation= DirectoryAttributeOperation.Add;
                schemaReload.Add("1");

                rqMod.Modifications.Add(schemaReload);

                //we don't sent result of this operation to pipeline
                conn.SendRequest(rqMod);

                //modify computer object
                rqMod = new ModifyRequest();
                rqMod.DistinguishedName = "cn=computer," + fi.SchemaNamingContext;
                DirectoryAttributeModification mayContain = new DirectoryAttributeModification();
                mayContain.Name = "mayContain";
                mayContain.Operation = DirectoryAttributeOperation.Add;
                mayContain.Add(Constants.PasswordAttributeName);
                mayContain.Add(Constants.TimestampAttributeName);
                rqMod.Modifications.Add(mayContain);

                try
                {
                    rspMod = conn.SendRequest(rqMod) as ModifyResponse;
                    WriteObject(new DirectoryOperationStatus(DirectoryOperationType.ModifySchemaClass, rqMod.DistinguishedName, rspMod.ResultCode));
                }
                catch (DirectoryOperationException ex)
                {
                    if (ex.Response.ResultCode == ResultCode.AttributeOrValueExists)
                        WriteObject(new DirectoryOperationStatus(DirectoryOperationType.ModifySchemaClass, rqMod.DistinguishedName, ex.Response.ResultCode));
                    else
                        throw;
                }

                //Reload schema
                rqMod = new ModifyRequest();

                rqMod.Modifications.Add(schemaReload);
                conn.SendRequest(rqMod);
            }
        }
    }

    [Cmdlet("Get", "AdmPwdPassword")]
    public class GetPassword : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 0, ValueFromPipeline = true)]
        public String ComputerName;

        protected override void ProcessRecord()
        {
            foreach (string dn in DirectoryUtils.GetComputerDN(ComputerName))
            {
                PasswordInfo pi = DirectoryUtils.GetPasswordInfo(dn);
                WriteObject(pi);
            }
        }
    }

    [Cmdlet("Reset", "AdmPwdPassword")]
    public class ResetPassword : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 0, ValueFromPipeline = true)]
        public String ComputerName;
        [Parameter(Mandatory = false, Position = 1, ValueFromPipeline = false)]
        public DateTime WhenEffective;

        protected override void ProcessRecord()
        {
            if (WhenEffective == DateTime.MinValue)
                WhenEffective = DateTime.Now;

            foreach (string dn in DirectoryUtils.GetComputerDN(ComputerName))
            {
                DirectoryUtils.ResetPassword(dn, WhenEffective);
                WriteObject(new PasswordResetStatus(dn, PasswordResetState.PasswordReset));
            }
        }
    }

    [Cmdlet("Set", "AdmPwdComputerSelfPermission")]
    public class DelegateComputerSelfPermission : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 0, ValueFromPipeline = true)]
        [Alias("OrgUnit")]
        public String Identity;


        protected LdapConnection conn = null;
        protected ForestInfo forestRootDomain;

        protected override void BeginProcessing()
        {
            conn = DirectoryUtils.GetLdapConnection(ConnectionType.Ldap);
            forestRootDomain = DirectoryUtils.GetForestRootDomain();

            base.BeginProcessing();
        }

        protected override void ProcessRecord()
        {
            //OU can be passed as name or as dn
            var OUs = DirectoryUtils.GetOU(Identity);
            if (OUs.Count > 1)
            {
                foreach (ObjectInfo ou in OUs)
                    WriteObject(ou);
                throw new AmbiguousResultException("More than one object found, search using distinguishedName instead");
            }
            if(OUs.Count==0)
                throw new NotFoundException($"Object not found: {Identity}");


            ActiveDirectorySecurity sec = DirectoryUtils.GetObjectSecurity(conn, OUs[0].DistinguishedName, System.DirectoryServices.Protocols.SecurityMasks.Dacl);
            //SELF SID
            System.Security.Principal.SecurityIdentifier selfSid = new System.Security.Principal.SecurityIdentifier("PS");
            //apply permissions only to computer objects
            Guid inheritedObjectGuid = DirectoryUtils.GetSchemaGuid(conn, forestRootDomain.SchemaNamingContext, "computer", SchemaObjectType.Class);
            System.DirectoryServices.PropertyAccessRule rule;
            Guid attributeGuid;

            // Read + Write ms-Mcs-AdmPwdExpirationTime on computer objects
            try
            {
                attributeGuid = DirectoryUtils.GetSchemaGuid(conn, forestRootDomain.SchemaNamingContext, Constants.TimestampAttributeName, SchemaObjectType.Attribute);
            }
            catch (Exception)
            {
                throw new NotFoundException($"Object not found: {Constants.TimestampAttributeName}");
            }
            rule = new System.DirectoryServices.PropertyAccessRule(selfSid,
                System.Security.AccessControl.AccessControlType.Allow,
                PropertyAccess.Read,
                attributeGuid, ActiveDirectorySecurityInheritance.Descendents,
                inheritedObjectGuid
                );
            sec.AddAccessRule(rule);
            rule = new System.DirectoryServices.PropertyAccessRule(selfSid,
                System.Security.AccessControl.AccessControlType.Allow,
                PropertyAccess.Write,
                attributeGuid, ActiveDirectorySecurityInheritance.Descendents,
                inheritedObjectGuid
                );
            sec.AddAccessRule(rule);

            // Write ms-Mcs-AdmPwd on computer objects
            try
            {
                attributeGuid = DirectoryUtils.GetSchemaGuid(conn, forestRootDomain.SchemaNamingContext, Constants.PasswordAttributeName, SchemaObjectType.Attribute);
            }
            catch (Exception)
            {
                throw new NotFoundException($"Object not found: {Constants.PasswordAttributeName}");
            }
            rule = new System.DirectoryServices.PropertyAccessRule(selfSid,
                System.Security.AccessControl.AccessControlType.Allow,
                PropertyAccess.Write,
                attributeGuid, ActiveDirectorySecurityInheritance.Descendents,
                inheritedObjectGuid
                );

            sec.AddAccessRule(rule);
            DirectoryUtils.SetObjectSecurity(conn, OUs[0].DistinguishedName, sec, System.DirectoryServices.Protocols.SecurityMasks.Dacl);
                
            WriteObject(OUs[0]);
            
        }

        protected override void EndProcessing()
        {
            if (conn != null)
            {
                conn.Dispose();
            }

            base.EndProcessing();
        }
    }

    [Cmdlet("Find", "AdmPwdExtendedRights")]
    public class FindExtendedRights : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 0, ValueFromPipeline = true)]
        [Alias("OrgUnit")]
        public String Identity;
        [Parameter(Mandatory = false, Position = 1, ValueFromPipeline = false)]
        public SwitchParameter IncludeComputers;

        protected LdapConnection conn = null;
        protected ForestInfo forestRootDomain;

        protected override void BeginProcessing()
        {
            conn = DirectoryUtils.GetLdapConnection(ConnectionType.Ldap);
            forestRootDomain = DirectoryUtils.GetForestRootDomain();

            base.BeginProcessing();
        }

        protected override void ProcessRecord()
        {
            //OU can be passed as name or as dn
            var OUs = DirectoryUtils.GetOU(Identity);
            if (OUs.Count > 1)
            {
                foreach (ObjectInfo ou in OUs)
                    WriteObject(ou);
                throw new AmbiguousResultException("More than one object found, search using distinguishedName instead");
            }
            if (OUs.Count == 0)
                throw new NotFoundException("Object not found");

            string filter;
            if(IncludeComputers)
                filter = "(|(objectCategory=container)(objectCategory=organizationalUnit)(objectCategory=computer))";
            else
                filter = "(|(objectCategory=container)(objectCategory=organizationalUnit))";
            var objectsToAnalyze = DirectoryUtils.Search(filter, OUs[0].DistinguishedName, "distinguishedName",500);
            foreach (string dn in objectsToAnalyze)
            {
                SearchRequest rq=new SearchRequest(dn,string.Format(System.Globalization.CultureInfo.InvariantCulture,"(distinguishedName={0})", dn),System.DirectoryServices.Protocols.SearchScope.Base,"ntSecurityDescriptor");
                rq.Controls.Add(new SecurityDescriptorFlagControl(System.DirectoryServices.Protocols.SecurityMasks.Dacl));

                SearchResponse rsp=conn.SendRequest(rq) as SearchResponse;

                ActiveDirectorySecurity acl= new ActiveDirectorySecurity();

                foreach (SearchResultEntry sr in rsp.Entries)
                {
                    byte[] ntSecurityDescriptor = sr.Attributes["ntsecuritydescriptor"].GetValues(typeof(byte[]))[0] as byte[];
                    acl.SetSecurityDescriptorBinaryForm(ntSecurityDescriptor);

                    List<string> data = new List<string>();
                    foreach (ActiveDirectoryAccessRule ace in acl.GetAccessRules(true, false, typeof(System.Security.Principal.NTAccount)))
                    {
                        if ((ace.ActiveDirectoryRights & System.DirectoryServices.ActiveDirectoryRights.ExtendedRight) == System.DirectoryServices.ActiveDirectoryRights.ExtendedRight)
                        {
                            string holder=ace.IdentityReference.ToString();
                            if(!data.Contains(holder))
                                data.Add(holder);
                        }
                    }
                    if (data.Count > 0)
                        WriteObject(new ExtendedRightsInfo(dn,data));
                }

            }
        }

        protected override void EndProcessing()
        {
            if (conn != null)
            {
                conn.Dispose();
            }

            base.EndProcessing();
        }
    }
    [Cmdlet("Set", "AdmPwdAuditing")]
    public class SetAuditing : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 0, ValueFromPipeline = true)]
        [Alias("OrgUnit")]
        public String Identity;
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = false)]
        public String[] AuditedPrincipals;
        [Parameter(Mandatory = false, Position = 2, ValueFromPipeline = false)]
        public System.Security.AccessControl.AuditFlags AuditType = System.Security.AccessControl.AuditFlags.Success;

        protected LdapConnection conn = null;
        protected ForestInfo forestRootDomain;

        protected override void BeginProcessing()
        {
            conn = DirectoryUtils.GetLdapConnection(ConnectionType.Ldap);
            forestRootDomain = DirectoryUtils.GetForestRootDomain();

            base.BeginProcessing();
        }

        protected override void ProcessRecord()
        {
            //OU can be passed as name or as dn
            var OUs = DirectoryUtils.GetOU(Identity);
            if (OUs.Count > 1)
            {
                foreach (ObjectInfo ou in OUs)
                    WriteObject(ou);
                throw new AmbiguousResultException("More than one object found, search using distinguishedName instead");
            }
            if (OUs.Count == 0)
                throw new NotFoundException("Object not found");


            ActiveDirectorySecurity sec = DirectoryUtils.GetObjectSecurity(conn, OUs[0].DistinguishedName, System.DirectoryServices.Protocols.SecurityMasks.Sacl);
            //apply permissions only to computer objects
            Guid inheritedObjectGuid = DirectoryUtils.GetSchemaGuid(conn, forestRootDomain.SchemaNamingContext, "computer", SchemaObjectType.Class);
            Guid pwdGuid = DirectoryUtils.GetSchemaGuid(conn, forestRootDomain.SchemaNamingContext, Constants.PasswordAttributeName, SchemaObjectType.Attribute);

            System.DirectoryServices.ActiveDirectoryAuditRule auditRule;

            foreach (string principalName in AuditedPrincipals)
            {
                System.Security.Principal.NTAccount principal = new System.Security.Principal.NTAccount(principalName);

                auditRule = new ActiveDirectoryAuditRule(
                    principal, 
                    ActiveDirectoryRights.ExtendedRight,
                    AuditType, 
                    pwdGuid,
                    ActiveDirectorySecurityInheritance.Descendents, 
                    inheritedObjectGuid 
                );
                sec.AddAuditRule(auditRule);
            }
            DirectoryUtils.SetObjectSecurity(conn, OUs[0].DistinguishedName, sec, System.DirectoryServices.Protocols.SecurityMasks.Sacl);

            WriteObject(OUs[0]);
        }

        protected override void EndProcessing()
        {
            if (conn != null)
            {
                conn.Dispose();
            }

            base.EndProcessing();
        }
    }
    
    [Cmdlet("Set", "AdmPwdReadPasswordPermission")]
    public class DelegateReadPasswordPermission : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 0, ValueFromPipeline = true)]
        [Alias("OrgUnit")]
        public String Identity;
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = false)]
        public String[] AllowedPrincipals;


        LdapConnection conn = null;
        ForestInfo forestRootDomain;

        protected override void BeginProcessing()
        {
            conn = DirectoryUtils.GetLdapConnection(ConnectionType.Ldap);
            forestRootDomain = DirectoryUtils.GetForestRootDomain();

            base.BeginProcessing();
        }

        protected override void ProcessRecord()
        {
            //OU can be passed as name or as dn
            var OUs = DirectoryUtils.GetOU(Identity);
            if (OUs.Count > 1)
            {
                foreach (ObjectInfo ou in OUs)
                    WriteObject(ou);
                throw new AmbiguousResultException("More than one object found, search using distinguishedName instead");
            }

            if (OUs.Count == 0)
                throw new NotFoundException($"Object not found: {Identity}");

            ActiveDirectorySecurity sec = DirectoryUtils.GetObjectSecurity(conn, OUs[0].DistinguishedName, System.DirectoryServices.Protocols.SecurityMasks.Dacl);
            
            //apply permissions only to computer objects
            Guid inheritedObjectGuid = DirectoryUtils.GetSchemaGuid(conn, forestRootDomain.SchemaNamingContext, "computer", SchemaObjectType.Class);
            Guid timestampGuid;
            Guid pwdGuid;
            try
            {
                timestampGuid = DirectoryUtils.GetSchemaGuid(conn, forestRootDomain.SchemaNamingContext, Constants.TimestampAttributeName, SchemaObjectType.Attribute);
            }
            catch (Exception)
            {
                throw new NotFoundException($"Object not found: {Constants.TimestampAttributeName}");
            }
            try
            {
                pwdGuid = DirectoryUtils.GetSchemaGuid(conn, forestRootDomain.SchemaNamingContext, Constants.PasswordAttributeName, SchemaObjectType.Attribute);
            }
            catch (Exception)
            {
                throw new NotFoundException($"Object not found: {Constants.PasswordAttributeName}");
            }

            System.DirectoryServices.ActiveDirectoryAccessRule rule;
            foreach (string principalName in AllowedPrincipals)
            {
                System.Security.Principal.NTAccount principal = new System.Security.Principal.NTAccount(principalName);

                // read ms-Mcs-AdmPwdExpirationTime on computer objects
                rule = new System.DirectoryServices.PropertyAccessRule(principal,
                    System.Security.AccessControl.AccessControlType.Allow,
                    PropertyAccess.Read,
                    timestampGuid, ActiveDirectorySecurityInheritance.Descendents,
                    inheritedObjectGuid
                   );
                sec.AddAccessRule(rule);

                // read ms-Mcs-AdmPwd on computer objects
                rule = new System.DirectoryServices.PropertyAccessRule(principal,
                    System.Security.AccessControl.AccessControlType.Allow,
                    PropertyAccess.Read,
                    pwdGuid, ActiveDirectorySecurityInheritance.Descendents,
                    inheritedObjectGuid
                   );
                sec.AddAccessRule(rule);

                // control access on ms-Mcs-AdmPwd on computer objects
                rule = new System.DirectoryServices.ActiveDirectoryAccessRule(principal,
                    ActiveDirectoryRights.ExtendedRight,
                    System.Security.AccessControl.AccessControlType.Allow,
                    pwdGuid, ActiveDirectorySecurityInheritance.Descendents,
                    inheritedObjectGuid
                   );

                sec.AddAccessRule(rule);
            }
            DirectoryUtils.SetObjectSecurity(conn, OUs[0].DistinguishedName, sec, System.DirectoryServices.Protocols.SecurityMasks.Dacl);
            WriteObject(OUs[0]);

        }

        protected override void EndProcessing()
        {
            if (conn != null)
            {
                conn.Dispose();
            }

            base.EndProcessing();
        }
    }

    [Cmdlet("Set", "AdmPwdResetPasswordPermission")]
    public class DelegateResetPasswordPermission : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 0, ValueFromPipeline = true)]
        [Alias("OrgUnit")]
        public String Identity;
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = false)]
        public String[] AllowedPrincipals;


        protected LdapConnection conn = null;
        protected ForestInfo forestRootDomain;

        protected override void BeginProcessing()
        {
            conn = DirectoryUtils.GetLdapConnection(ConnectionType.Ldap);
            forestRootDomain = DirectoryUtils.GetForestRootDomain();

            base.BeginProcessing();
        }

        protected override void ProcessRecord()
        {
            //OU can be passed as name or as dn
            var OUs = DirectoryUtils.GetOU(Identity);
            if (OUs.Count > 1)
            {
                foreach (ObjectInfo ou in OUs)
                    WriteObject(ou);
                throw new AmbiguousResultException("More than one object found, search using distinguishedName instead");
            }

            if (OUs.Count == 0)
                throw new NotFoundException("Object not found");

            ActiveDirectorySecurity sec = DirectoryUtils.GetObjectSecurity(conn, OUs[0].DistinguishedName, System.DirectoryServices.Protocols.SecurityMasks.Dacl);
            //apply permissions only to computer objects
            Guid inheritedObjectGuid = DirectoryUtils.GetSchemaGuid(conn, forestRootDomain.SchemaNamingContext, "computer", SchemaObjectType.Class);
            Guid timestampGuid = DirectoryUtils.GetSchemaGuid(conn, forestRootDomain.SchemaNamingContext, Constants.TimestampAttributeName, SchemaObjectType.Attribute);

            //System.DirectoryServices.PropertyAccessRule rule;
            System.DirectoryServices.ActiveDirectoryAccessRule rule;
            foreach (string principalName in AllowedPrincipals)
            {
                System.Security.Principal.NTAccount principal = new System.Security.Principal.NTAccount(principalName);

                // read ms-Mcs-AdmPwdExpirationTime on computer objects
                rule = new System.DirectoryServices.PropertyAccessRule(principal,
                    System.Security.AccessControl.AccessControlType.Allow,
                    PropertyAccess.Read,
                    timestampGuid, ActiveDirectorySecurityInheritance.Descendents,
                    inheritedObjectGuid
                   );
                sec.AddAccessRule(rule);

                // write ms-Mcs-AdmPwdExpirationTime on computer objects
                rule = new System.DirectoryServices.PropertyAccessRule(principal,
                    System.Security.AccessControl.AccessControlType.Allow,
                    PropertyAccess.Write,
                    timestampGuid, ActiveDirectorySecurityInheritance.Descendents,
                    inheritedObjectGuid
                   );
                sec.AddAccessRule(rule);
            }
            DirectoryUtils.SetObjectSecurity(conn, OUs[0].DistinguishedName, sec, System.DirectoryServices.Protocols.SecurityMasks.Dacl);

            WriteObject(OUs[0]);
        }

        protected override void EndProcessing()
        {
            if (conn != null)
            {
                conn.Dispose();
            }
            base.EndProcessing();
        }
    }
}

