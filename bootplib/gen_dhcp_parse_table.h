/*
 * Copyright (c) 1999-2024 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*
 * This file was auto-generated by ./genoptionfiles -table, do not edit!
 */
#ifndef _S_DHCP_PARSE_TABLE
#define _S_DHCP_PARSE_TABLE
static const dhcptag_info_t dhcptag_info_table[] = {
  /*   0 */ { dhcptype_none_e     , "pad" },
  /*   1 */ { dhcptype_ip_e       , "subnet_mask" },
  /*   2 */ { dhcptype_int32_e    , "time_offset" },
  /*   3 */ { dhcptype_ip_mult_e  , "router" },
  /*   4 */ { dhcptype_ip_mult_e  , "time_server" },
  /*   5 */ { dhcptype_ip_mult_e  , "name_server" },
  /*   6 */ { dhcptype_ip_mult_e  , "domain_name_server" },
  /*   7 */ { dhcptype_ip_mult_e  , "log_server" },
  /*   8 */ { dhcptype_ip_mult_e  , "cookie_server" },
  /*   9 */ { dhcptype_ip_mult_e  , "lpr_server" },
  /*  10 */ { dhcptype_ip_mult_e  , "impress_server" },
  /*  11 */ { dhcptype_ip_mult_e  , "resource_location_server" },
  /*  12 */ { dhcptype_string_e   , "host_name" },
  /*  13 */ { dhcptype_uint16_e   , "boot_file_size" },
  /*  14 */ { dhcptype_string_e   , "merit_dump_file" },
  /*  15 */ { dhcptype_string_e   , "domain_name" },
  /*  16 */ { dhcptype_ip_e       , "swap_server" },
  /*  17 */ { dhcptype_string_e   , "root_path" },
  /*  18 */ { dhcptype_string_e   , "extensions_path" },
  /*  19 */ { dhcptype_bool_e     , "ip_forwarding" },
  /*  20 */ { dhcptype_bool_e     , "non_local_source_routing" },
  /*  21 */ { dhcptype_ip_pairs_e , "policy_filter" },
  /*  22 */ { dhcptype_uint16_e   , "max_dgram_reassembly_size" },
  /*  23 */ { dhcptype_uint8_e    , "default_ip_time_to_live" },
  /*  24 */ { dhcptype_uint32_e   , "path_mtu_aging_timeout" },
  /*  25 */ { dhcptype_uint16_mult_e, "path_mtu_plateau_table" },
  /*  26 */ { dhcptype_uint16_e   , "interface_mtu" },
  /*  27 */ { dhcptype_bool_e     , "all_subnets_local" },
  /*  28 */ { dhcptype_ip_e       , "broadcast_address" },
  /*  29 */ { dhcptype_bool_e     , "perform_mask_discovery" },
  /*  30 */ { dhcptype_bool_e     , "mask_supplier" },
  /*  31 */ { dhcptype_bool_e     , "perform_router_discovery" },
  /*  32 */ { dhcptype_ip_e       , "router_solicitation_address" },
  /*  33 */ { dhcptype_ip_pairs_e , "static_route" },
  /*  34 */ { dhcptype_bool_e     , "trailer_encapsulation" },
  /*  35 */ { dhcptype_uint32_e   , "arp_cache_timeout" },
  /*  36 */ { dhcptype_bool_e     , "ethernet_encapsulation" },
  /*  37 */ { dhcptype_uint8_e    , "default_ttl" },
  /*  38 */ { dhcptype_uint32_e   , "keepalive_interval" },
  /*  39 */ { dhcptype_bool_e     , "keepalive_garbage" },
  /*  40 */ { dhcptype_string_e   , "nis_domain" },
  /*  41 */ { dhcptype_ip_mult_e  , "nis_servers" },
  /*  42 */ { dhcptype_ip_mult_e  , "network_time_protocol_servers" },
  /*  43 */ { dhcptype_opaque_e   , "vendor_specific" },
  /*  44 */ { dhcptype_ip_mult_e  , "nb_over_tcpip_name_server" },
  /*  45 */ { dhcptype_ip_mult_e  , "nb_over_tcpip_dgram_dist_server" },
  /*  46 */ { dhcptype_uint8_e    , "nb_over_tcpip_node_type" },
  /*  47 */ { dhcptype_string_e   , "nb_over_tcpip_scope" },
  /*  48 */ { dhcptype_ip_mult_e  , "x_windows_font_server" },
  /*  49 */ { dhcptype_ip_mult_e  , "x_windows_display_manager" },
  /*  50 */ { dhcptype_ip_e       , "requested_ip_address" },
  /*  51 */ { dhcptype_uint32_e   , "lease_time" },
  /*  52 */ { dhcptype_uint8_e    , "option_overload" },
  /*  53 */ { dhcptype_uint8_e    , "dhcp_message_type" },
  /*  54 */ { dhcptype_ip_e       , "server_identifier" },
  /*  55 */ { dhcptype_uint8_mult_e, "parameter_request_list" },
  /*  56 */ { dhcptype_string_e   , "message" },
  /*  57 */ { dhcptype_uint16_e   , "max_dhcp_message_size" },
  /*  58 */ { dhcptype_uint32_e   , "renewal_t1_time_value" },
  /*  59 */ { dhcptype_uint32_e   , "rebinding_t2_time_value" },
  /*  60 */ { dhcptype_string_e   , "vendor_class_identifier" },
  /*  61 */ { dhcptype_uint8_mult_e, "client_identifier" },
  /*  62 */ { dhcptype_opaque_e   , "option_62" },
  /*  63 */ { dhcptype_opaque_e   , "option_63" },
  /*  64 */ { dhcptype_string_e   , "nis_plus_domain" },
  /*  65 */ { dhcptype_ip_mult_e  , "nis_plus_servers" },
  /*  66 */ { dhcptype_ip_mult_e  , "tftp_server_name" },
  /*  67 */ { dhcptype_string_e   , "bootfile_name" },
  /*  68 */ { dhcptype_ip_mult_e  , "mobile_ip_home_agent" },
  /*  69 */ { dhcptype_ip_mult_e  , "smtp_server" },
  /*  70 */ { dhcptype_ip_mult_e  , "pop3_server" },
  /*  71 */ { dhcptype_ip_mult_e  , "nntp_server" },
  /*  72 */ { dhcptype_ip_mult_e  , "default_www_server" },
  /*  73 */ { dhcptype_ip_mult_e  , "default_finger_server" },
  /*  74 */ { dhcptype_ip_mult_e  , "default_irc_server" },
  /*  75 */ { dhcptype_ip_mult_e  , "streettalk_server" },
  /*  76 */ { dhcptype_ip_mult_e  , "stda_server" },
  /*  77 */ { dhcptype_opaque_e   , "option_77" },
  /*  78 */ { dhcptype_opaque_e   , "slp_directory_agent" },
  /*  79 */ { dhcptype_opaque_e   , "slp_service_scope" },
  /*  80 */ { dhcptype_opaque_e   , "option_80" },
  /*  81 */ { dhcptype_opaque_e   , "option_81" },
  /*  82 */ { dhcptype_opaque_e   , "option_82" },
  /*  83 */ { dhcptype_opaque_e   , "option_83" },
  /*  84 */ { dhcptype_opaque_e   , "option_84" },
  /*  85 */ { dhcptype_opaque_e   , "option_85" },
  /*  86 */ { dhcptype_opaque_e   , "option_86" },
  /*  87 */ { dhcptype_opaque_e   , "option_87" },
  /*  88 */ { dhcptype_opaque_e   , "option_88" },
  /*  89 */ { dhcptype_opaque_e   , "option_89" },
  /*  90 */ { dhcptype_opaque_e   , "option_90" },
  /*  91 */ { dhcptype_opaque_e   , "option_91" },
  /*  92 */ { dhcptype_opaque_e   , "option_92" },
  /*  93 */ { dhcptype_opaque_e   , "option_93" },
  /*  94 */ { dhcptype_opaque_e   , "option_94" },
  /*  95 */ { dhcptype_string_e   , "ldap_url" },
  /*  96 */ { dhcptype_opaque_e   , "option_96" },
  /*  97 */ { dhcptype_opaque_e   , "option_97" },
  /*  98 */ { dhcptype_opaque_e   , "option_98" },
  /*  99 */ { dhcptype_opaque_e   , "option_99" },
  /* 100 */ { dhcptype_string_e   , "posix_timezone" },
  /* 101 */ { dhcptype_string_e   , "tzdb_timezone" },
  /* 102 */ { dhcptype_opaque_e   , "option_102" },
  /* 103 */ { dhcptype_opaque_e   , "option_103" },
  /* 104 */ { dhcptype_opaque_e   , "option_104" },
  /* 105 */ { dhcptype_opaque_e   , "option_105" },
  /* 106 */ { dhcptype_opaque_e   , "option_106" },
  /* 107 */ { dhcptype_opaque_e   , "option_107" },
  /* 108 */ { dhcptype_uint32_e   , "ipv6_only_preferred" },
  /* 109 */ { dhcptype_opaque_e   , "option_109" },
  /* 110 */ { dhcptype_opaque_e   , "option_110" },
  /* 111 */ { dhcptype_opaque_e   , "option_111" },
  /* 112 */ { dhcptype_ip_mult_e  , "netinfo_server_address" },
  /* 113 */ { dhcptype_string_e   , "netinfo_server_tag" },
  /* 114 */ { dhcptype_string_e   , "captive_portal_url" },
  /* 115 */ { dhcptype_opaque_e   , "option_115" },
  /* 116 */ { dhcptype_uint8_e    , "auto_configure" },
  /* 117 */ { dhcptype_opaque_e   , "option_117" },
  /* 118 */ { dhcptype_opaque_e   , "option_118" },
  /* 119 */ { dhcptype_dns_namelist_e, "domain_search" },
  /* 120 */ { dhcptype_opaque_e   , "option_120" },
  /* 121 */ { dhcptype_classless_route_e, "classless_static_route" },
  /* 122 */ { dhcptype_opaque_e   , "option_122" },
  /* 123 */ { dhcptype_opaque_e   , "option_123" },
  /* 124 */ { dhcptype_opaque_e   , "option_124" },
  /* 125 */ { dhcptype_opaque_e   , "option_125" },
  /* 126 */ { dhcptype_opaque_e   , "option_126" },
  /* 127 */ { dhcptype_opaque_e   , "option_127" },
  /* 128 */ { dhcptype_opaque_e   , "option_128" },
  /* 129 */ { dhcptype_opaque_e   , "option_129" },
  /* 130 */ { dhcptype_opaque_e   , "option_130" },
  /* 131 */ { dhcptype_opaque_e   , "option_131" },
  /* 132 */ { dhcptype_opaque_e   , "option_132" },
  /* 133 */ { dhcptype_opaque_e   , "option_133" },
  /* 134 */ { dhcptype_opaque_e   , "option_134" },
  /* 135 */ { dhcptype_opaque_e   , "option_135" },
  /* 136 */ { dhcptype_opaque_e   , "option_136" },
  /* 137 */ { dhcptype_opaque_e   , "option_137" },
  /* 138 */ { dhcptype_opaque_e   , "option_138" },
  /* 139 */ { dhcptype_opaque_e   , "option_139" },
  /* 140 */ { dhcptype_opaque_e   , "option_140" },
  /* 141 */ { dhcptype_opaque_e   , "option_141" },
  /* 142 */ { dhcptype_opaque_e   , "option_142" },
  /* 143 */ { dhcptype_opaque_e   , "option_143" },
  /* 144 */ { dhcptype_opaque_e   , "option_144" },
  /* 145 */ { dhcptype_opaque_e   , "option_145" },
  /* 146 */ { dhcptype_opaque_e   , "option_146" },
  /* 147 */ { dhcptype_opaque_e   , "option_147" },
  /* 148 */ { dhcptype_opaque_e   , "option_148" },
  /* 149 */ { dhcptype_opaque_e   , "option_149" },
  /* 150 */ { dhcptype_opaque_e   , "option_150" },
  /* 151 */ { dhcptype_opaque_e   , "option_151" },
  /* 152 */ { dhcptype_opaque_e   , "option_152" },
  /* 153 */ { dhcptype_opaque_e   , "option_153" },
  /* 154 */ { dhcptype_opaque_e   , "option_154" },
  /* 155 */ { dhcptype_opaque_e   , "option_155" },
  /* 156 */ { dhcptype_opaque_e   , "option_156" },
  /* 157 */ { dhcptype_opaque_e   , "option_157" },
  /* 158 */ { dhcptype_opaque_e   , "option_158" },
  /* 159 */ { dhcptype_opaque_e   , "option_159" },
  /* 160 */ { dhcptype_opaque_e   , "option_160" },
  /* 161 */ { dhcptype_opaque_e   , "option_161" },
  /* 162 */ { dhcptype_opaque_e   , "option_162" },
  /* 163 */ { dhcptype_opaque_e   , "option_163" },
  /* 164 */ { dhcptype_opaque_e   , "option_164" },
  /* 165 */ { dhcptype_opaque_e   , "option_165" },
  /* 166 */ { dhcptype_opaque_e   , "option_166" },
  /* 167 */ { dhcptype_opaque_e   , "option_167" },
  /* 168 */ { dhcptype_opaque_e   , "option_168" },
  /* 169 */ { dhcptype_opaque_e   , "option_169" },
  /* 170 */ { dhcptype_opaque_e   , "option_170" },
  /* 171 */ { dhcptype_opaque_e   , "option_171" },
  /* 172 */ { dhcptype_opaque_e   , "option_172" },
  /* 173 */ { dhcptype_opaque_e   , "option_173" },
  /* 174 */ { dhcptype_opaque_e   , "option_174" },
  /* 175 */ { dhcptype_opaque_e   , "option_175" },
  /* 176 */ { dhcptype_opaque_e   , "option_176" },
  /* 177 */ { dhcptype_opaque_e   , "option_177" },
  /* 178 */ { dhcptype_opaque_e   , "option_178" },
  /* 179 */ { dhcptype_opaque_e   , "option_179" },
  /* 180 */ { dhcptype_opaque_e   , "option_180" },
  /* 181 */ { dhcptype_opaque_e   , "option_181" },
  /* 182 */ { dhcptype_opaque_e   , "option_182" },
  /* 183 */ { dhcptype_opaque_e   , "option_183" },
  /* 184 */ { dhcptype_opaque_e   , "option_184" },
  /* 185 */ { dhcptype_opaque_e   , "option_185" },
  /* 186 */ { dhcptype_opaque_e   , "option_186" },
  /* 187 */ { dhcptype_opaque_e   , "option_187" },
  /* 188 */ { dhcptype_opaque_e   , "option_188" },
  /* 189 */ { dhcptype_opaque_e   , "option_189" },
  /* 190 */ { dhcptype_opaque_e   , "option_190" },
  /* 191 */ { dhcptype_opaque_e   , "option_191" },
  /* 192 */ { dhcptype_opaque_e   , "option_192" },
  /* 193 */ { dhcptype_opaque_e   , "option_193" },
  /* 194 */ { dhcptype_opaque_e   , "option_194" },
  /* 195 */ { dhcptype_opaque_e   , "option_195" },
  /* 196 */ { dhcptype_opaque_e   , "option_196" },
  /* 197 */ { dhcptype_opaque_e   , "option_197" },
  /* 198 */ { dhcptype_opaque_e   , "option_198" },
  /* 199 */ { dhcptype_opaque_e   , "option_199" },
  /* 200 */ { dhcptype_opaque_e   , "option_200" },
  /* 201 */ { dhcptype_opaque_e   , "option_201" },
  /* 202 */ { dhcptype_opaque_e   , "option_202" },
  /* 203 */ { dhcptype_opaque_e   , "option_203" },
  /* 204 */ { dhcptype_opaque_e   , "option_204" },
  /* 205 */ { dhcptype_opaque_e   , "option_205" },
  /* 206 */ { dhcptype_opaque_e   , "option_206" },
  /* 207 */ { dhcptype_opaque_e   , "option_207" },
  /* 208 */ { dhcptype_opaque_e   , "option_208" },
  /* 209 */ { dhcptype_opaque_e   , "option_209" },
  /* 210 */ { dhcptype_opaque_e   , "option_210" },
  /* 211 */ { dhcptype_opaque_e   , "option_211" },
  /* 212 */ { dhcptype_opaque_e   , "option_212" },
  /* 213 */ { dhcptype_opaque_e   , "option_213" },
  /* 214 */ { dhcptype_opaque_e   , "option_214" },
  /* 215 */ { dhcptype_opaque_e   , "option_215" },
  /* 216 */ { dhcptype_opaque_e   , "option_216" },
  /* 217 */ { dhcptype_opaque_e   , "option_217" },
  /* 218 */ { dhcptype_opaque_e   , "option_218" },
  /* 219 */ { dhcptype_opaque_e   , "option_219" },
  /* 220 */ { dhcptype_opaque_e   , "option_220" },
  /* 221 */ { dhcptype_opaque_e   , "option_221" },
  /* 222 */ { dhcptype_opaque_e   , "option_222" },
  /* 223 */ { dhcptype_opaque_e   , "option_223" },
  /* 224 */ { dhcptype_opaque_e   , "option_224" },
  /* 225 */ { dhcptype_opaque_e   , "option_225" },
  /* 226 */ { dhcptype_opaque_e   , "option_226" },
  /* 227 */ { dhcptype_opaque_e   , "option_227" },
  /* 228 */ { dhcptype_opaque_e   , "option_228" },
  /* 229 */ { dhcptype_opaque_e   , "option_229" },
  /* 230 */ { dhcptype_opaque_e   , "option_230" },
  /* 231 */ { dhcptype_opaque_e   , "option_231" },
  /* 232 */ { dhcptype_opaque_e   , "option_232" },
  /* 233 */ { dhcptype_opaque_e   , "option_233" },
  /* 234 */ { dhcptype_opaque_e   , "option_234" },
  /* 235 */ { dhcptype_opaque_e   , "option_235" },
  /* 236 */ { dhcptype_opaque_e   , "option_236" },
  /* 237 */ { dhcptype_opaque_e   , "option_237" },
  /* 238 */ { dhcptype_opaque_e   , "option_238" },
  /* 239 */ { dhcptype_opaque_e   , "option_239" },
  /* 240 */ { dhcptype_opaque_e   , "option_240" },
  /* 241 */ { dhcptype_opaque_e   , "option_241" },
  /* 242 */ { dhcptype_opaque_e   , "option_242" },
  /* 243 */ { dhcptype_opaque_e   , "option_243" },
  /* 244 */ { dhcptype_opaque_e   , "option_244" },
  /* 245 */ { dhcptype_opaque_e   , "option_245" },
  /* 246 */ { dhcptype_opaque_e   , "option_246" },
  /* 247 */ { dhcptype_opaque_e   , "option_247" },
  /* 248 */ { dhcptype_opaque_e   , "option_248" },
  /* 249 */ { dhcptype_opaque_e   , "option_249" },
  /* 250 */ { dhcptype_opaque_e   , "option_250" },
  /* 251 */ { dhcptype_opaque_e   , "option_251" },
  /* 252 */ { dhcptype_string_e   , "proxy_auto_discovery_url" },
  /* 253 */ { dhcptype_opaque_e   , "option_253" },
  /* 254 */ { dhcptype_opaque_e   , "option_254" },
  /* 255 */ { dhcptype_none_e     , "end" },
};

static const dhcptype_info_t dhcptype_info_table[] = {
  /*  0 */ { 0, dhcptype_none_e, "none", 0 },
  /*  1 */ { 0, dhcptype_none_e, "opaque", 0 },
  /*  2 */ { 1, dhcptype_none_e, "bool", 0 },
  /*  3 */ { 1, dhcptype_none_e, "uint8", 0 },
  /*  4 */ { 2, dhcptype_none_e, "uint16", 0 },
  /*  5 */ { 4, dhcptype_none_e, "uint32", 0 },
  /*  6 */ { 4, dhcptype_none_e, "int32", 0 },
  /*  7 */ { 1, dhcptype_uint8_e, "uint8_mult", 1 },
  /*  8 */ { 2, dhcptype_uint16_e, "uint16_mult", 1 },
  /*  9 */ { 0, dhcptype_none_e, "string", 0 },
  /* 10 */ { 4, dhcptype_none_e, "ip", 0 },
  /* 11 */ { 4, dhcptype_ip_e, "ip_mult", 1 },
  /* 12 */ { 8, dhcptype_ip_e, "ip_pairs", 1 },
  /* 13 */ { 0, dhcptype_none_e, "dns_namelist", 1 },
  /* 14 */ { 0, dhcptype_none_e, "classless_route", 1 },
};
#endif /* _S_DHCP_PARSE_TABLE */
