const Footer = () => {
  return (
    <footer className="w-full h-[580px] bg-blue-800 py-6 flex items-center justify-center text-white">
      <div className="">
        <span className="flex gap-2.5 items-center justify-around">
          <img src="/assets/issa_white.png" alt="logo" className="w-[100px]" />
          <span>|</span>
          <h3 className="text-2xl font-semibold">E-Learning Platform</h3>
        </span>

        {/* subscribe */}
        {/* <div className="text-center">
          <h3 className="text-gray-500">Subscribe to get our Newsletter</h3>
          <input
            type="email"
            className="border border-slate-300 bg-transparent mt-3 px-4 py-2 rounded-full"
            placeholder="Your Email"
          />
          <button
            type="submit"
            className="ml-3 px-6 py-2 bg-amber-600 font-semibold text-white rounded-full"
          >
            Subscribe
          </button>
        </div> */}

        {/* Navigator */}
        <div className="mt-6 text-slate-500">
          <div className="flex items-center justify-around">
            <span>Careers</span>
            <span>|</span>
            <span>Privacy Policy</span>
            <span>|</span>
            <span>Terms & Conditions</span>
          </div>
          <div className="text-center mt-3">
            &copy; 2024 Class Technologies Inc.
          </div>
        </div>
      </div>
    </footer>
  );
};
export default Footer;
