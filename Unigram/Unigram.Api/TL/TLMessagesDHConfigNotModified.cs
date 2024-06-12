// <auto-generated/>
using System;

namespace Telegram.Api.TL
{
	public partial class TLMessagesDHConfigNotModified : TLMessagesDHConfigBase 
	{
		public TLMessagesDHConfigNotModified() { }
		public TLMessagesDHConfigNotModified(TLBinaryReader from)
		{
			Read(from);
		}

		public override TLType TypeId { get { return TLType.MessagesDHConfigNotModified; } }

		public override void Read(TLBinaryReader from)
		{
			Random = from.ReadByteArray();
		}

		public override void Write(TLBinaryWriter to)
		{
			to.Write(0xC0E24635);
			to.WriteByteArray(Random);
		}
	}
}