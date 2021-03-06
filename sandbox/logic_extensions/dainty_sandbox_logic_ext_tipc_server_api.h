/******************************************************************************

MIT License

Copyright (c) 2018 frits.germs@gmx.net

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************/

#ifndef _DAINTY_SANDBOX_LOGIC_EXT_TIPC_SERVER_API_H_
#define _DAINTY_SANDBOX_LOGIC_EXT_TIPC_SERVER_API_H_

namespace dainty
{
namespace sandbox
{
namespace logic_tipc_server_ext
{
///////////////////////////////////////////////////////////////////////////////

  class t_logic_tipc_server_ext_api {
  public:
    virtual ~t_logic_tipc_server_ext_api() {}

    /*
    virtual t_tipc_server_id add_tipc_server(t_err, R_tipc_server_params) noexcept = 0;
    virtual t_tipc_server_id add_tipc_server(t_err, R_tipc_server_params,
                                             x_tipc_server_notify_ptr)    noexcept = 0;

    virtual t_tipc_server_notify_ptr del_tipc_server(t_tipc_server_id)       noexcept = 0;
    virtual P_tipc_server_params     get_tipc_server(t_tipc_server_id) const noexcept = 0;

    virtual t_bool         del_tipc_server_client(t_tipc_client_id)       noexcept = 0;
    virtual P_tipc_address get_tipc_server_client(t_tipc_client_id) const noexcept = 0;

    virtual t_void send_tipc_server_message(t_err, t_tipc_client_id,
                                            R_message) noexcept = 0;
    virtual t_void send_tipc_server_message(t_err, t_tipc_server_id,
                                            R_tipc_address,
                                            R_message) noexcept = 0;
    */
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
