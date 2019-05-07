#include <siderelay.hpp>
#include <chain.hpp>

// ontransfer
// if memo is "" so support user transfer token to self account in relay chain
// if memo is "xxx", memo is the account to transfer in relay chain
// if memo is "xxx|tt", xxx is the account to transfer in relay chain tt is memo
void siderelay::ontransfer( capi_name from, capi_name to, const asset& quantity, const std::string& memo ) {
   if( name(from) == _self || name(to) != _self ) {
      return;
   }
   if( "NoProcessMemo" == memo ) {
      return;
   }

   print("on transfer ", name(from), " -> ", name(to), " ", quantity, " by ", memo, "\n");

   siderelay::in_action in(_self, { _self, "active"_n });

   auto to_account = to;
   if( !memo.empty() ) {
      to_account = name{memo}.value;
   }

   in.send(1, to_account, quantity, "to relay chain");
}

// from side chain to relay
ACTION siderelay::in( uint64_t num,  capi_name to, const asset& quantity, const std::string& memo ) {
   // print("in ", from, " ", to, " ", quantity, "\n");
   // TODO By FanYang check num if ok by relay chain
}

// from relay chain to side
ACTION siderelay::out( capi_name committer, 
                       uint64_t num, 
                       capi_name to, 
                       capi_name chain, 
                       capi_name contract, 
                       capi_name action, 
                       const asset& quantity, 
                       const std::string& memo ) {
   if( !WORK_CHECK( out, committer, 
            to, chain, contract, action, quantity, memo ) ){
      return;
   }

   print("do_out ", to, ",", chain, ",", quantity, ",", memo, "\n");
   chainspec::send_token(name{contract}, name{action}, _self, name{to}, quantity, memo );
}